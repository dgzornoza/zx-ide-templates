# Draw functions y tipos de sprite — SP1

## Elección del draw function

La función de dibujo (`drawf`) se pasa a `sp1_CreateSpr`, `sp1_AddColSpr` o `sp1_ChangeSprType` y determina cómo SP1 combina el gráfico del sprite con el fondo en cada celda.

### Tabla de decisión rápida

| Necesidad | Draw function | `type` |
|---|---|---|
| Sprite con fondo transparente (típico) | `SP1_DRAW_MASK2` | `SP1_TYPE_2BYTE` |
| Igual pero sin rotación por software | `SP1_DRAW_MASK2NR` | `SP1_TYPE_2BYTE` |
| Sprite opaco (sobreescribe fondo) | `SP1_DRAW_LOAD2` | `SP1_TYPE_2BYTE` |
| Sprite sin máscara (1 byte/pixel, más ligero) | `SP1_DRAW_LOAD1` | `SP1_TYPE_1BYTE` |
| Efecto XOR sobre el fondo | `SP1_DRAW_XOR2` | `SP1_TYPE_2BYTE` |
| Efecto OR sobre el fondo | `SP1_DRAW_OR2` | `SP1_TYPE_2BYTE` |
| Solo dibujar atributos (no píxeles) | `SP1_DRAW_ATTR` | cualquiera |

### Variantes de bordes (LB / RB)

SP1 usa bordes izquierdo (`LB`) y derecho (`RB`) para el semi-tile exterior cuando el sprite no está alineado a tile:

| Función | Posición |
|---|---|
| `SP1_DRAW_MASK2LB` | Borde izquierdo con máscara |
| `SP1_DRAW_MASK2RB` | Borde derecho con máscara |
| `SP1_DRAW_LOAD2LB` | Borde izquierdo carga directa |
| `SP1_DRAW_LOAD2RB` | Borde derecho carga directa |
| `SP1_DRAW_LOAD2LBIM` | Borde izquierdo con máscara implícita |
| `SP1_DRAW_LOAD2RBIM` | Borde derecho con máscara implícita |

> Para sprites de una sola columna usar `LB` en `sp1_CreateSpr` (borde izquierdo) y `RB` en el último `sp1_AddColSpr` (borde derecho).

---

## Tipos de sprite (`SP1_TYPE_*`)

```c
SP1_TYPE_2BYTE   /* formato (mask, data) por byte — con máscara */
SP1_TYPE_1BYTE   /* formato (data) solo — sin máscara */
SP1_TYPE_OCCLUDE /* el sprite oculta fondo y sprites por debajo */
SP1_TYPE_BGNDCLR /* +occlude: copia tile de fondo al pixel buffer antes de dibujar */
```

Combinaciones habituales:

```c
SP1_TYPE_2BYTE                          /* sprite con máscara */
SP1_TYPE_2BYTE | SP1_TYPE_OCCLUDE       /* opaco con máscara */
SP1_TYPE_1BYTE | SP1_TYPE_OCCLUDE       /* opaco sin máscara */
SP1_TYPE_1BYTE | SP1_TYPE_OCCLUDE | SP1_TYPE_BGNDCLR  /* occlude + limpia fondo */
```

---

## Patrón estándar para sprite 2×2 con máscara

```
          col 0          col 1
row 0  [ MASK2LB ]   [ MASK2RB ]
row 1  [ MASK2LB ]   [ MASK2RB ]
       (ficticio)    (ficticio)  ← height + 1 en sp1_CreateSpr
```

```c
extern uint8_t spr_hero[];
#define H       2               /* tiles de alto */
#define W_BYTES (H * 8 * 2)    /* bytes por columna (mask+data, 1 frame) */

struct sp1_ss *create_hero(void) {
    struct sp1_ss *s = sp1_CreateSpr(
        SP1_DRAW_MASK2LB, SP1_TYPE_2BYTE,
        H + 1,                  /* height + 1 (col ficticia) */
        (int)spr_hero, 0);      /* offset frame 0 col 0, plano 0 */

    sp1_AddColSpr(s,
        SP1_DRAW_MASK2RB, SP1_TYPE_2BYTE,
        (int)(spr_hero + W_BYTES), 0);   /* col 1 */

    return s;
}
```

---

## Formato de datos gráficos en el .asm

### Sprite con máscara (`SP1_TYPE_2BYTE`)

Formato: **máscara** (1=transparente, 0=opaco), luego **dato** por fila, por columna, por frame.

```asm
; 1 frame, 1 columna, 2 tiles de alto (16 filas)
_spr_hero:
; frame 0, columna 0
    defb 0xE7, 0x00   ; fila 0:  máscara | dato
    defb 0xC3, 0x3C   ; fila 1
    defb 0x81, 0x7E   ; fila 2
    ; ... 16 filas total (8 por tile × 2 tiles)
```

### Sprite sin máscara (`SP1_TYPE_1BYTE`)

Formato: solo **dato** por fila.

```asm
_spr_bullet:
    defb 0x18   ; fila 0
    defb 0x18   ; fila 1
    ; ...
```

---

## Rotación por software (`SP1_IFLAG_MAKE_ROTTBL`)

Cuando se usa `SP1_DRAW_MASK2` (sin `NR`), SP1 genera rotaciones software para posicionamiento sub-tile en X (hrot). Requiere:

1. `SP1_IFLAG_MAKE_ROTTBL` en `sp1_Initialize`.
2. `SP1_TYPE_2BYTE` en el sprite.
3. Usar `sp1_MoveSprPix` o pasar `hrot` distinto de 0 en `sp1_MoveSprAbs`.

Sin rotación (`NR`): el sprite siempre aparece alineado al borde de tile; más rápido pero movimiento en pasos de 8px en X.

---

## Umbrales de sprite (`xthresh` / `ythresh`)

```c
spr->xthresh = 1;  /* dibuja columna extra al desplazarse 1+ px horiz. */
spr->ythresh = 1;  /* dibuja fila extra al desplazarse 1+ px vert. */
```

Valores por defecto: 1. Aumentar si el sprite parece recortado en los bordes.

---

## `sp1_PreShiftSpr` — frames pre-desplazados

Para animaciones con movimiento de 1 píxel muy frecuente, pre-calcular todas las variantes desplazadas acelera el bucle de juego:

```c
/* Reservar espacio para 8 versiones desplazadas (hrot 0..7) */
static uint8_t shifted_frames[8][FRAME_BYTES];

void preshift_all(void) {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        sp1_PreShiftSpr(0, H, W_COL, (void *)spr_hero,
                        (void *)shifted_frames[i], i);
    }
}

/* Uso en el bucle: seleccionar frame según (px & 7) */
sp1_MoveSprPix(spr, &clip,
               (void *)shifted_frames[px & 7], px >> 3, py >> 3);
/* Nota: con PreShift el hrot siempre es 0 */
```
