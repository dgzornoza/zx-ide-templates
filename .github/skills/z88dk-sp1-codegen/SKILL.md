---
name: z88dk-sp1-codegen
description: >-
  Genera código C y ASM para la librería gráfica SP1 de z88dk orientado a juegos
  en ZX Spectrum. Usa cuando: crear sprites con máscara, inicializar SP1, mover
  sprites por píxel, gestionar tiles/UDGs, bucle de juego, colisiones, optimización
  de memoria en 8 bits, scaffolding de módulos, configuración zpragma.inc,
  patrones SDCC/z88dk-iy, variables globales, __z88dk_fastcall, IM2.
argument-hint: "Describe qué necesitas: sprite, tile, colisión, bucle de juego, etc."
---

# z88dk SP1 Code Generator

Genera código C y ASM para la librería gráfica **SP1 v3** de z88dk, compilando con SDCC (`sdcc_iy`), para juegos en **ZX Spectrum 48K/128K**.

## Cuándo usar

- Crear o ampliar sprites (masked, 1-byte, 2-byte, multi-columna, animados)
- Inicializar SP1 y configurar `zpragma.inc`
- Gestionar tiles y UDGs de fondo
- Implementar bucle de juego con `sp1_UpdateNow`
- Detectar colisiones (hotspot, bounding box, pixel-perfect)
- Optimizar memoria: variables globales, `static`, pools, sin `malloc`
- Scaffolding de nuevos módulos (moduleN.c / modules.h / main.c)
- Usar `__z88dk_fastcall`, callbacks y punteros a función

## Principios de diseño (8 bits)

1. **Sin malloc/free** — usar pool estático (`CLIB_BALLOC_TABLE_SIZE`) o arrays globales.
2. **Variables globales y `static`** — el stack es de 128 bytes; evitar paso de structs por valor.
3. **`__z88dk_fastcall`** para funciones con un solo argumento (en `HL`); reduce código generado.
4. **Minimizar uso de `printf`** en bucles — impacta el rendimiento; usar `sp1_PrintAtInv` o `sp1_PrintString`.
5. **Declarar `const` en ROM** cuando sea posible para evitar copias a RAM.
6. **Attribute clash** — diseñar sprites en monocromo o con máscara; los colores se gestionan por celda 8×8.
7. **Tipos explícitos** — preferir `uint8_t`, `uint16_t`, `int8_t` (`<stdint.h>`); evitar `int` en hotpaths.

---

## Patrón 1: Estructura de proyecto y zpragma.inc

```c
/* zpragma.inc — configuración mínima para SP1 */
#pragma output CRT_ORG_CODE           = 25000
#pragma output REGISTER_SP            = 0xd000
#pragma output CRT_ENABLE_CLOSE       = 0
#pragma output CRT_ENABLE_EIDI        = 0x1
#pragma output CRT_ON_EXIT            = 0
#pragma output CLIB_STDIO_HEAP_SIZE   = 0
#pragma output CLIB_EXIT_STACK_SIZE   = 0
#pragma output CLIB_FOPEN_MAX         = -1
#pragma output CLIB_OPEN_MAX          = -1
#pragma output CLIB_BALLOC_TABLE_SIZE = 1   /* OBLIGATORIO para sp1_CreateSpr */
#pragma output CRT_STACK_SIZE         = 128
```

> `CLIB_BALLOC_TABLE_SIZE = 1` es **obligatorio** para que SP1 pueda asignar sprites dinámicamente.

---

## Patrón 2: Cabeceras z88dk

```c
/* z88dk_headers.h */
#include <arch/zx.h>
#include <arch/zx/sp1.h>
#include <intrinsic.h>
#include <z80.h>
#include <stdint.h>
```

---

## Patrón 3: Inicialización SP1

```c
static struct sp1_Rect full_screen = {0, 0, 32, 24};

void init_sp1(void) {
    sp1_Initialize(
        SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
        INK_WHITE | PAPER_BLACK,
        ' '
    );
    sp1_Invalidate(&full_screen);   /* forzar primer dibujado completo */
    sp1_UpdateNow();
}
```

---

## Patrón 4: Definición de UDGs / Tiles de fondo

```c
/* En el archivo .asm generado por la herramienta de exportación */
/* SECTION rodata_compiler */
/* _tile_wall: defb 0x3C, 0x7E, 0xFF, ... (8 bytes) */

extern uint8_t tile_wall[];   /* declarado en el .h generado */

void setup_tiles(void) {
    sp1_TileEntry('A', tile_wall);   /* asigna tile_wall al carácter 'A' */
    /* sp1_PrintAtInv(row, col, INK_GREEN | PAPER_BLACK, 'A'); */
}
```

---

## Patrón 5: Crear un sprite con máscara (2 columnas × 2 filas)

```c
/* En el .asm del sprite: pares (mask, data) por fila, por columna, por frame */
extern uint8_t spr_player[];

static struct sp1_ss *player;

/* Offset entre columnas: height_tiles × 8 bytes × 2 (mask+data) × num_frames */
#define COL_OFFSET  32   /* 2 tiles alto × 8 bytes × 2 bytes/pixel × 1 frame */

void create_player_sprite(void) {
    /* columna 0 */
    player = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE, 3,
                           (int)spr_player, 0);
    /* columna 1 */
    sp1_AddColSpr(player, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE,
                  (int)(spr_player + COL_OFFSET), 0);
    /* Sin más columnas: terminar con SP1_TYPE_2BYTE | SP1_TYPE_OCCLUDE si es oclusivo */
}
```

> Para sprites de 1 columna usar solo `sp1_CreateSpr`. El último `sp1_AddColSpr` puede marcar fin de sprite.

---

## Patrón 6: Colores por celda del sprite

```c
/* Hook llamado por sp1_IterateSprChar para asignar color a cada sp1_cs */
static void set_sprite_color(uint16_t count, struct sp1_cs *c) {
    (void)count;
    c->attr_mask = SP1_AMASK_TRANS;   /* 0xFF — transparente: no cambia atributos */
    c->attr      = INK_WHITE | PAPER_BLACK;
}

/* Llamar tras crear el sprite */
sp1_IterateSprChar(player, set_sprite_color);
```

---

## Patrón 7: Movimiento por píxel

```c
static uint8_t px = 0, py = 0;   /* coordenadas en píxeles */
static struct sp1_Rect clip = {0, 0, 32, 24};

void move_player(int8_t dx, int8_t dy) {
    px += dx;
    py += dy;
    sp1_MoveSprPix(player, &clip, (void *)spr_player, px, py);
}
```

---

## Patrón 8: Bucle de juego principal

```c
void run_game(void) {
    init_sp1();
    create_player_sprite();
    sp1_IterateSprChar(player, set_sprite_color);
    sp1_MoveSprPix(player, &clip, (void *)spr_player, 0, 0);

    while (1) {
        /* 1. Leer input */
        /* 2. Actualizar lógica */
        /* 3. Mover sprites */
        /* 4. Refrescar pantalla */
        sp1_UpdateNow();
    }
}
```

---

## Patrón 9: Callback `__z88dk_fastcall`

```c
/* Recepción del callback — argumento único en HL */
void update_callback(void) __z88dk_fastcall;

void wait_for_key(void (*cb)(void)) __z88dk_fastcall {
    uint8_t key;
    do {
        cb();
        key = in_inkey();
    } while (key == 0);
}
```

---

## Patrón 10: Detección de colisiones por bounding box

```c
typedef struct {
    int16_t x1, y1, x2, y2;
} BBox;

uint8_t bbox_collide(const BBox *a, const BBox *b) {
    return !(b->x1 >= a->x2 || b->x2 <= a->x1 ||
             b->y1 >= a->y2 || b->y2 <= a->y1);
}

/* Actualizar para el sprite en px, py con tamaño w, h */
void update_bbox(BBox *bb, int16_t x, int16_t y, uint8_t w, uint8_t h) {
    bb->x1 = x; bb->y1 = y;
    bb->x2 = x + w; bb->y2 = y + h;
}
```

---

## Patrón 11: Hotspot sobre atributos de pantalla

```c
uint8_t check_hotspot(uint8_t px, uint8_t py) {
    uint8_t attr = *zx_pxy2aaddr(px, py);
    return (attr != (INK_BLACK | PAPER_BLACK));   /* choca si no es fondo negro */
}
```

---

## Patrón 12: Scaffold de nuevo módulo

```c
/* src/modules/moduleN.c */
#include "../z88dk_headers.h"
#include "modules.h"
#include "../data/moduleN.h"   /* tiles/sprites del módulo */

static struct sp1_Rect full_screen = {0, 0, 32, 24};
static struct sp1_Rect clip        = {0, 0, 32, 24};

void run_exampleN(void) {
    sp1_Initialize(
        SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
        INK_WHITE | PAPER_BLACK, ' ');
    sp1_Invalidate(&full_screen);
    sp1_UpdateNow();

    /* --- setup --- */

    while (1) {
        /* --- lógica --- */
        sp1_UpdateNow();
    }
}
```

```c
/* Añadir en src/modules/modules.h */
void run_exampleN(void);
```

```c
/* Añadir caso en src/main.c switch */
case 'N' - '0': run_exampleN(); break;
```

---

## Referencia rápida SP1

Ver detalles completos en:
- [API de funciones SP1](./references/sp1-api.md)
- [Optimización de memoria](./references/memory-patterns.md)
- [Draw functions y tipos de sprite](./references/draw-functions.md)
