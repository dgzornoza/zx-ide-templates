# Optimización de memoria y rendimiento — ZX Spectrum 48K

Guía de patrones para maximizar el rendimiento y minimizar el uso de RAM en juegos con z88dk/SP1.

---

## Restricciones del hardware

| Recurso | ZX Spectrum 48K |
|---|---|
| RAM total | 48 KB |
| Stack (SP1) | 128 bytes (`CRT_STACK_SIZE`) |
| ROM | 16 KB (0x0000–0x3FFF, no disponible) |
| Pantalla | 6912 bytes (0x4000–0x57FF pixel + 0x5800–0x5AFF attrs) |
| Código + datos | ~25 KB (depende del origen: `CRT_ORG_CODE = 25000`) |
| Heap SP1 | Gestionado por `CLIB_BALLOC_TABLE_SIZE` |

---

## Reglas de oro

### 1. Nunca usar `malloc` / `free` en código de juego

SP1 gestiona su propio heap con `balloc` interno. Para datos del juego usar siempre arrays globales o `static`.

```c
/* MAL */
uint8_t *buffer = malloc(64);

/* BIEN */
static uint8_t buffer[64];
/* o global */
uint8_t buffer[64];
```

### 2. Pasar punteros, no structs por valor

El stack de 128 bytes se agota con facilidad si se pasan structs por valor.

```c
/* MAL — copia struct en el stack */
void process(struct sp1_Rect r) { ... }

/* BIEN — pasa puntero */
void process(const struct sp1_Rect *r) { ... }
```

### 3. Usar `uint8_t` y `uint16_t` en lugar de `int`

SDCC puede generar código más compacto con tipos explícitos de 8/16 bits.

```c
/* Preferir */
uint8_t  counter = 0;
uint16_t score   = 0;
int8_t   dx      = -1;    /* velocidad (puede ser negativa) */
```

### 4. `__z88dk_fastcall` para funciones con un argumento

Pasa el argumento en el par de registros HL, eliminando el overhead de la pila.

```c
/* Declaración */
void update(void *frame) __z88dk_fastcall;

/* Uso como callback */
void run(void (*cb)(void)) __z88dk_fastcall {
    cb();
}
```

### 5. Variables globales en lugar de locales para datos persistentes

Las variables locales se almacenan en el stack; las globales en la sección BSS/DATA.

```c
/* Estado del jugador — global, no local */
static uint8_t  player_x  = 0;
static uint8_t  player_y  = 0;
static uint16_t player_hp = 100;
```

### 6. Tablas de datos `const` en ROM/RODATA

Usar `const` para gráficos y tablas de nivel que no cambian.

```c
/* En el .asm exportado por la herramienta gráfica — sección rodata */
/* SECTION rodata_compiler */
/* _tile_floor: defb 0x00, ... */

/* En C */
extern const uint8_t tile_floor[];
```

### 7. No usar `printf` en el bucle de juego

`printf` y la biblioteca stdio añaden overhead importante. Usar funciones SP1:

```c
/* MAL en bucle de juego */
printf("Score: %d", score);

/* BIEN */
sp1_PrintAtInv(22, 0, INK_WHITE | PAPER_BLACK, 'S');
/* o con sp1_PrintString si el contexto está configurado */
```

---

## Gestión de animaciones sin heap

```c
/* Array de frames estático — los datos GFX viven en el .asm externo */
extern uint8_t spr_enemy[];        /* n_frames × height × 8 × bytes_per_pixel */

#define FRAME_SIZE  32             /* 2 tiles alto × 8 líneas × 2 bytes (mask+data) */
#define NUM_FRAMES  4

static uint8_t anim_frame = 0;
static uint8_t anim_tick  = 0;

void advance_animation(struct sp1_ss *spr, struct sp1_Rect *clip,
                       uint8_t x, uint8_t y) {
    if (++anim_tick >= 4) {
        anim_tick = 0;
        if (++anim_frame >= NUM_FRAMES) anim_frame = 0;
    }
    sp1_MoveSprPix(spr, clip,
                   (void *)(spr_enemy + anim_frame * FRAME_SIZE), x, y);
}
```

---

## Cálculo del offset de columnas en sprites multi-columna

Para un sprite de **W columnas × H tiles de alto**, con **N frames**:

```
offset_columna_k = k × H × 8 × bytes_por_pixel × N_frames
```

- Sprite con máscara (`SP1_TYPE_2BYTE`): `bytes_por_pixel = 2`
- Sprite sin máscara (`SP1_TYPE_1BYTE`): `bytes_por_pixel = 1`

```c
#define SPR_H       2     /* tiles de alto */
#define SPR_N_FRAMES 4
#define COL_STRIDE  (SPR_H * 8 * 2 * SPR_N_FRAMES)   /* 2BYTE */

struct sp1_ss *spr = sp1_CreateSpr(SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE,
                                    SPR_H + 1, (int)spr_data, 0);
sp1_AddColSpr(spr, SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE,
              (int)(spr_data + COL_STRIDE), 0);
```

---

## Gestión de múltiples enemigos con pool estático

```c
#define MAX_ENEMIES  8

typedef struct {
    struct sp1_ss *spr;
    uint8_t        x, y;
    uint8_t        active;
} Enemy;

static Enemy enemy_pool[MAX_ENEMIES];

void init_enemies(void) {
    uint8_t i;
    for (i = 0; i < MAX_ENEMIES; i++) {
        enemy_pool[i].spr    = sp1_CreateSpr(SP1_DRAW_MASK2LB,
                                              SP1_TYPE_2BYTE, 3, (int)enemy_gfx, 0);
        sp1_AddColSpr(enemy_pool[i].spr, SP1_DRAW_MASK2RB,
                      SP1_TYPE_2BYTE, (int)(enemy_gfx + COL_STRIDE), 0);
        enemy_pool[i].active = 0;
    }
}
```

---

## Alinear código crítico

z88dk/SDCC no garantiza alineación, pero algunos trucos ayudan:

- Colocar tablas de rotación/lookup al inicio de `SECTION rodata_compiler` en el .asm.
- Usar `#pragma codeseg` o secciones ASM nominadas para código crítico.

---

## Modo IM2 para bucle de 50 Hz (128K / AY)

Para un timing de 50 Hz se puede configurar IM2 con una ISR que establezca un flag:

```c
/* En zpragma.inc: CRT_ENABLE_EIDI = 0x1 desactiva interrupciones al inicio */
/* La ISR se registra vía el mecanismo de z88dk (ver docs CRT) */

volatile uint8_t vsync_flag = 0;

/* En el bucle principal */
while (!vsync_flag) { /* esperar */ }
vsync_flag = 0;
sp1_UpdateNow();
```

---

## Checklist de optimización para cada módulo

- [ ] ¿Todas las variables del estado del juego son globales o `static`?
- [ ] ¿Se evita pasar structs por valor?
- [ ] ¿Los gráficos están en secciones `rodata` del linker?
- [ ] ¿Se usa `uint8_t`/`uint16_t` en contadores y coordenadas?
- [ ] ¿El bucle llama `sp1_UpdateNow()` exactamente una vez por frame?
- [ ] ¿`CLIB_BALLOC_TABLE_SIZE = 1` está en `zpragma.inc`?
- [ ] ¿Hay `sp1_DeleteSpr` para sprites que se eliminan mid-game?
