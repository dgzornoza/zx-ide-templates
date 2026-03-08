# SP1 API — Referencia de funciones

Referencia completa de funciones agrupadas por categoría para z88dk SP1 v3.

---

## Inicialización y pantalla

| Función | Firma | Descripción |
|---|---|---|
| `sp1_Initialize` | `(uint16_t iflag, uint16_t colour, uint16_t tile)` | Inicializa SP1. `iflag`: combinación de `SP1_IFLAG_*`. |
| `sp1_Invalidate` | `(struct sp1_Rect *r)` | Marca rectángulo como sucio (fuerza redibujado). |
| `sp1_Validate` | `(struct sp1_Rect *r)` | Marca rectángulo como válido (no redibuja). |
| `sp1_UpdateNow` | `(void)` | Redibuja todas las celdas invalidadas. Llamar cada frame. |

**Flags de inicialización:**

- `SP1_IFLAG_MAKE_ROTTBL` — genera tabla de rotaciones software (necesario para `MoveSprPix`).
- `SP1_IFLAG_OVERWRITE_TILES` — sobrescribe tabla de tiles al inicializar.
- `SP1_IFLAG_OVERWRITE_DFILE` — borra el fichero de pantalla al inicializar.

---

## Sprites — creación y destrucción

```c
struct sp1_ss *sp1_CreateSpr(void *drawf, uint16_t type, uint16_t height,
                              int graphic, uint16_t plane);
uint16_t       sp1_AddColSpr(struct sp1_ss *s, void *drawf, uint16_t type,
                              int graphic, uint16_t plane);
void           sp1_ChangeSprType(struct sp1_cs *c, void *drawf);
void           sp1_DeleteSpr(struct sp1_ss *s);
```

- `height`: número de tiles de alto **+1** (columna ficticia de terminación).
- `plane`: 0 = más cercano al observador; 255 = encima del fondo.
- `sp1_AddColSpr`: llamar por cada columna adicional; el último cierra la lista.

---

## Sprites — movimiento

```c
void sp1_MoveSprAbs(struct sp1_ss *s, struct sp1_Rect *clip, void *frame,
                    uint16_t row, uint16_t col, uint16_t vrot, uint16_t hrot);
void sp1_MoveSprRel(struct sp1_ss *s, struct sp1_Rect *clip, void *frame,
                    int rel_row, int rel_col, int rel_vrot, int rel_hrot);
void sp1_MoveSprPix(struct sp1_ss *s, struct sp1_Rect *clip, void *frame,
                    uint16_t x, uint16_t y);
```

- `MoveSprAbs` / `MoveSprRel`: coordenadas en **tiles** (granularidad 8px).
- `MoveSprPix`: coordenadas en **píxeles**; SP1 ajusta tiles y rotaciones solo.
- `clip`: `sp1_Rect` que define los límites de clipping del sprite.
- `frame`: puntero al frame de animación activo (offset a datos gráficos).

---

## Sprites — colores e iteración

```c
void sp1_IterateSprChar(struct sp1_ss *s, void *hook);
/* hook: void hook(uint16_t count, struct sp1_cs *c)  */

void sp1_IterateUpdateSpr(struct sp1_ss *s, void *hook);
/* hook: void hook(uint16_t count, struct sp1_update *u) */

void sp1_GetSprClrAddr(struct sp1_ss *s, uint8_t **sprdest);
void sp1_PutSprClr(uint8_t **sprdest, struct sp1_ap *src, uint16_t n);
void sp1_GetSprClr(uint8_t **sprsrc,  struct sp1_ap *dest, uint16_t n);
```

**Ejemplo — asignar color a cada celda del sprite:**

```c
static void color_hook(uint16_t count, struct sp1_cs *c) {
    (void)count;
    c->attr_mask = SP1_AMASK_TRANS;
    c->attr      = INK_CYAN | PAPER_BLACK;
}
sp1_IterateSprChar(spr, color_hook);
```

**Máscaras de atributo:**

- `SP1_AMASK_TRANS` (0xFF) — totalmente transparente (hereda color del fondo).
- `SP1_AMASK_INK`   (0xF8) — solo tinta del sprite; paper del fondo.
- `SP1_AMASK_PAPER` (0xC7) — solo papel del sprite; tinta del fondo.
- `SP1_ATTR_TRANS`  (0x00) — atributo para sprite sin color propio.

---

## Tiles y texto

```c
void    *sp1_TileEntry(uint16_t c, void *def);
void     sp1_PrintAt(uint16_t row, uint16_t col, uint16_t colour, uint16_t tile);
void     sp1_PrintAtInv(uint16_t row, uint16_t col, uint16_t colour, uint16_t tile);
uint16_t sp1_ScreenStr(uint16_t row, uint16_t col);
uint16_t sp1_ScreenAttr(uint16_t row, uint16_t col);
void     sp1_PrintString(struct sp1_pss *ps, uint8_t *s);
void     sp1_SetPrintPos(struct sp1_pss *ps, uint16_t row, uint16_t col);
void     sp1_ClearRect(struct sp1_Rect *r, uint16_t colour, uint16_t tile, uint16_t rflag);
void     sp1_ClearRectInv(struct sp1_Rect *r, uint16_t colour, uint16_t tile, uint16_t rflag);
void     sp1_GetTiles(struct sp1_Rect *r, struct sp1_tp *dest);
void     sp1_PutTiles(struct sp1_Rect *r, struct sp1_tp *src);
void     sp1_PutTilesInv(struct sp1_Rect *r, struct sp1_tp *src);
```

**Flags de rectángulo (`rflag`):**

- `SP1_RFLAG_TILE`   — limpia/actualiza tiles.
- `SP1_RFLAG_COLOUR` — limpia/actualiza atributos de color.
- `SP1_RFLAG_SPRITE` — limpia/actualiza sprite list.

**Flags de impresión (`sp1_pss.flags`):**

- `SP1_PSSFLAG_INVALIDATE` — invalida celdas escritas.
- `SP1_PSSFLAG_XWRAP`      — wrap horizontal al llegar al borde.
- `SP1_PSSFLAG_YINC`       — avanza cursor en Y al imprimir.
- `SP1_PSSFLAG_YWRAP`      — wrap vertical.

**Ejemplo — imprimir cadena con atributo:**

```c
static struct sp1_Rect text_area = {2, 0, 32, 1};
static struct sp1_pss  ps = {
    &text_area, SP1_PSSFLAG_INVALIDATE | SP1_PSSFLAG_XWRAP,
    0, 0, 0xFF, INK_WHITE | PAPER_BLACK, 0, 0
};
sp1_SetPrintPos(&ps, 0, 0);
sp1_PrintString(&ps, (uint8_t *)"SCORE: 0000");
```

---

## Estructuras principales

### `struct sp1_Rect`

```c
struct sp1_Rect { uint8_t row, col, width, height; };
```

### `struct sp1_ap` — par atributo/máscara para colores de sprite

```c
struct sp1_ap { uint8_t attr_mask; uint8_t attr; };
```

### `struct sp1_tp` — tile/atributo de fondo (para GetTiles/PutTiles)

```c
struct sp1_tp { uint8_t attr; uint16_t tile; };
```

### `struct sp1_pss` — estado de impresión de cadenas

```c
struct sp1_pss {
    struct sp1_Rect *bounds;
    uint8_t flags;
    uint8_t x, y;
    uint8_t attr_mask, attr;
    struct sp1_update *pos;
    void *visit;
};
```

---

## Updater — control fino del redibujado

```c
struct sp1_update *sp1_GetUpdateStruct(uint16_t row, uint16_t col);
void sp1_InvUpdateStruct(struct sp1_update *u);
void sp1_ValUpdateStruct(struct sp1_update *u);
void sp1_RemoveUpdateStruct(struct sp1_update *u);
void sp1_RestoreUpdateStruct(struct sp1_update *u);
void sp1_DrawUpdateStructIfInv(struct sp1_update *u);
void sp1_DrawUpdateStructAlways(struct sp1_update *u);
void sp1_IterateUpdateRect(struct sp1_Rect *r, void *hook);
void sp1_IterateUpdateArr(struct sp1_update **ua, void *hook);
```

---

## Char structs independientes

Útiles para efectos estáticos sin crear un sprite completo:

```c
void sp1_InitCharStruct(struct sp1_cs *cs, void *drawf, uint16_t type,
                        void *graphic, uint16_t plane);
void sp1_InsertCharStruct(struct sp1_update *u, struct sp1_cs *cs);
void sp1_RemoveCharStruct(struct sp1_cs *cs);
```

**Ejemplo:**

```c
static struct sp1_cs hud_char;
sp1_InitCharStruct(&hud_char, SP1_DRAW_OR1NR, SP1_TYPE_1BYTE, icon_gfx, 0);
sp1_InsertCharStruct(sp1_GetUpdateStruct(0, 30), &hud_char);
sp1_InvUpdateStruct(sp1_GetUpdateStruct(0, 30));
```
