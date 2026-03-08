# Z88dk SP1 (Sprite Pack v3.0) – Documentación

Documentación práctica de las funciones principales de la libreria SP1 para ZX Spectrum. Incluye descripción y ejemplo mínimo por función. Las funciones están agrupadas por funcionalidad para facilitar la búsqueda.

> Nota: Los ejemplos son fragmentos ilustrativos. Ajustar tipos, tiles y gráficos según se requiera.

---

## Tabla de contenidos (TOC)

- [Estructuras de datos](#estructuras-de-datos)
  - [sp1_Rect](#sp1_rect)
  - [sp1_update](#sp1_update)
  - [sp1_ss](#sp1_ss)
  - [sp1_cs](#sp1_cs)
  - [sp1_ap](#sp1_ap)
  - [sp1_tp](#sp1_tp)
  - [sp1_pss](#sp1_pss)
- [Constantes y flags](#constantes-y-flags)
  - [Tipos y atributos de sprites](#tipos-y-atributos-de-sprites)
  - [Flags de rectángulo y printing](#flags-de-rectángulo-y-printing)
  - [Flags de inicialización (updater)](#flags-de-inicialización-updater)
- [Referencia de defines y estructuras](#referencia-de-defines-y-estructuras)
  - [Defines y constantes](#defines-y-constantes)
  - [Funciones de dibujo (draw functions)](#funciones-de-dibujo-draw-functions)
  - [Estructuras (campos y descripción)](#estructuras-campos-y-descripción)
- [Prototypes públicos (funciones y macros)](#prototypes-públicos-funciones-y-macros)
  - [Tabla TOC rápida (funciones)](#tabla-toc-rápida-funciones)
- [Flujo de inicialización recomendado](#flujo-de-inicialización-recomendado)
- [Sprites: creación y configuración](#sprites-creación-y-configuración)
  - [sp1_CreateSpr](#sp1_createspr)
  - [sp1_AddColSpr](#sp1_addcolspr)
  - [sp1_ChangeSprType](#sp1_changesprtype)
  - [sp1_DeleteSpr](#sp1_deletespr)
- [Sprites: movimiento](#sprites-movimiento)
  - [sp1_MoveSprAbs](#sp1_movesprabs)
  - [sp1_MoveSprRel](#sp1_movesprrel)
  - [sp1_MoveSprPix](#sp1_movesprpix)
- [Sprites: iteración y utilidades](#sprites-iteración-y-utilidades)
  - [sp1_IterateSprChar](#sp1_iteratesprchar)
  - [sp1_IterateUpdateSpr](#sp1_iterateupdatespr)
  - [sp1_GetSprClrAddr](#sp1_getsprclraddr)
  - [sp1_PutSprClr](#sp1_putsprclr)
  - [sp1_GetSprClr](#sp1_getsprclr)
  - [sp1_PreShiftSpr](#sp1_preshiftspr)
- [Char structs independientes](#char-structs-independientes)
  - [sp1_InitCharStruct](#sp1_initcharstruct)
  - [sp1_InsertCharStruct](#sp1_insertcharstruct)
  - [sp1_RemoveCharStruct](#sp1_removecharstruct)
- [Tiles y texto](#tiles-y-texto)
  - [sp1_TileEntry](#sp1_tileentry)
  - [sp1_PrintAt](#sp1_printat)
  - [sp1_PrintAtInv](#sp1_printatinv)
  - [sp1_ScreenStr](#sp1_screenstr)
  - [sp1_ScreenAttr](#sp1_screenattr)
  - [sp1_PrintString](#sp1_printstring)
  - [sp1_SetPrintPos](#sp1_setprintpos)
  - [sp1_GetTiles](#sp1_gettiles)
  - [sp1_PutTiles](#sp1_puttiles)
  - [sp1_PutTilesInv](#sp1_puttilesinv)
  - [sp1_ClearRect](#sp1_clearrect)
  - [sp1_ClearRectInv](#sp1_clearrectinv)
- [Updater (actualización de pantalla)](#updater-actualización-de-pantalla)
  - [sp1_Initialize](#sp1_initialize)
  - [sp1_UpdateNow](#sp1_updatenow)
  - [sp1_GetUpdateStruct](#sp1_getupdatestruct)
  - [sp1_IterateUpdateArr](#sp1_iterateupdatearr)
  - [sp1_IterateUpdateRect](#sp1_iterateupdaterect)
  - [sp1_InvUpdateStruct](#sp1_invupdatestruct)
  - [sp1_ValUpdateStruct](#sp1_valupdatestruct)
  - [sp1_DrawUpdateStructIfInv](#sp1_drawupdatestructifinv)
  - [sp1_DrawUpdateStructIfVal](#sp1_drawupdatestructifval)
  - [sp1_DrawUpdateStructIfNotRem](#sp1_drawupdatestructifnotrem)
  - [sp1_DrawUpdateStructAlways](#sp1_drawupdatestructalways)
  - [sp1_RemoveUpdateStruct](#sp1_removeupdatestruct)
  - [sp1_RestoreUpdateStruct](#sp1_restoreupdatestruct)
  - [sp1_Invalidate](#sp1_invalidate)
  - [sp1_Validate](#sp1_validate)
- [Funciones de dibujo (draw functions)](#funciones-de-dibujo-draw-functions)
  - [SP1_DRAW_MASK2](#sp1_draw_mask2)
  - [SP1_DRAW_MASK2NR](#sp1_draw_mask2nr)
  - [SP1_DRAW_MASK2LB](#sp1_draw_mask2lb)
  - [SP1_DRAW_MASK2RB](#sp1_draw_mask2rb)
  - [SP1_DRAW_LOAD2](#sp1_draw_load2)
  - [SP1_DRAW_LOAD2NR](#sp1_draw_load2nr)
  - [SP1_DRAW_LOAD2LB](#sp1_draw_load2lb)
  - [SP1_DRAW_LOAD2RB](#sp1_draw_load2rb)
  - [SP1_DRAW_OR2](#sp1_draw_or2)
  - [SP1_DRAW_OR2NR](#sp1_draw_or2nr)
  - [SP1_DRAW_OR2LB](#sp1_draw_or2lb)
  - [SP1_DRAW_OR2RB](#sp1_draw_or2rb)
  - [SP1_DRAW_XOR2](#sp1_draw_xor2)
  - [SP1_DRAW_XOR2NR](#sp1_draw_xor2nr)
  - [SP1_DRAW_XOR2LB](#sp1_draw_xor2lb)
  - [SP1_DRAW_XOR2RB](#sp1_draw_xor2rb)
  - [SP1_DRAW_LOAD2LBIM](#sp1_draw_load2lbim)
  - [SP1_DRAW_LOAD2RBIM](#sp1_draw_load2rbim)
  - [SP1_DRAW_LOAD1](#sp1_draw_load1)
  - [SP1_DRAW_LOAD1NR](#sp1_draw_load1nr)
  - [SP1_DRAW_LOAD1LB](#sp1_draw_load1lb)
  - [SP1_DRAW_LOAD1RB](#sp1_draw_load1rb)
  - [SP1_DRAW_OR1](#sp1_draw_or1)
  - [SP1_DRAW_OR1NR](#sp1_draw_or1nr)
  - [SP1_DRAW_OR1LB](#sp1_draw_or1lb)
  - [SP1_DRAW_OR1RB](#sp1_draw_or1rb)
  - [SP1_DRAW_XOR1](#sp1_draw_xor1)
  - [SP1_DRAW_XOR1NR](#sp1_draw_xor1nr)
  - [SP1_DRAW_XOR1LB](#sp1_draw_xor1lb)
  - [SP1_DRAW_XOR1RB](#sp1_draw_xor1rb)
  - [SP1_DRAW_LOAD1LBIM](#sp1_draw_load1lbim)
  - [SP1_DRAW_LOAD1RBIM](#sp1_draw_load1rbim)
  - [SP1_DRAW_ATTR](#sp1_draw_attr)

---

## Estructuras de datos

### sp1_Rect
Rectángulo en coordenadas de tiles (fila, columna, ancho, alto). Se usa para límites de impresión, invalidación y operaciones en tiles.

**Ejemplo:**
```c
struct sp1_Rect clip = { .row = 4, .col = 6, .width = 12, .height = 8 };
```

### sp1_update
Describe cada tile gestionado por SP1. Contiene estado de invalidación, color de fondo, tile, lista de sprites y puntero al área de pantalla.

**Ejemplo:**
```c
struct sp1_update *u = sp1_GetUpdateStruct(10, 10);
u->colour = 0x47;
```

### sp1_ss
Estructura principal del sprite. Guarda posición en tiles, tamaño, rotación, frame actual y lista de `sp1_cs`.

**Ejemplo:**
```c
struct sp1_ss *spr = sp1_CreateSpr(SP1_DRAW_MASK2, SP1_TYPE_2BYTE, 2, (int)gfx, 0);
spr->xthresh = 1;
```

### sp1_cs
“Sprite char” (pieza del sprite que cabe en un tile). Incluye atributos, plano, punteros a funciones de dibujo y enlaces en listas.

**Ejemplo:**
```c
struct sp1_cs cs;
sp1_InitCharStruct(&cs, SP1_DRAW_OR1, SP1_TYPE_1BYTE, gfx_tile, 0);
```

### sp1_ap
Par atributo/máscara para colores de sprite.

**Ejemplo:**
```c
struct sp1_ap ap = { .attr_mask = 0xFF, .attr = 0x47 };
```

### sp1_tp
Par tile/atributo para operaciones con fondo.

**Ejemplo:**
```c
struct sp1_tp tp = { .attr = 0x47, .tile = 'A' };
```

### sp1_pss
Estado de impresión de texto: límites, posición de cursor y atributos.

**Ejemplo:**
```c
struct sp1_pss ps = { &clip, SP1_PSSFLAG_INVALIDATE, 0, 0, 0xFF, 0x47, 0, 0 };
```

---

## Constantes y flags

### Tipos y atributos de sprites
- `SP1_TYPE_OCCLUDE`: el sprite oculta fondo y sprites debajo.
- `SP1_TYPE_BGNDCLR`: para sprites occlusivos, limpia con tile de fondo.
- `SP1_TYPE_2BYTE` / `SP1_TYPE_1BYTE`: formato de gráfico (con máscara o sin máscara).
- Máscaras de atributo: `SP1_AMASK_TRANS`, `SP1_AMASK_INK`, `SP1_AMASK_PAPER`, `SP1_AMASK_NOFLASH`, `SP1_AMASK_NOBRIGHT`.
- Atributo transparente: `SP1_ATTR_TRANS`.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_MASK2, SP1_TYPE_2BYTE | SP1_TYPE_OCCLUDE, 2, (int)gfx, 0);
```

### Flags de rectángulo y printing
- `SP1_RFLAG_TILE`, `SP1_RFLAG_COLOUR`, `SP1_RFLAG_SPRITE`: controlan qué se limpia o actualiza en rectángulos.
- `SP1_PSSFLAG_INVALIDATE`, `SP1_PSSFLAG_XWRAP`, `SP1_PSSFLAG_YINC`, `SP1_PSSFLAG_YWRAP`: control del comportamiento de impresión.

**Ejemplo:**
```c
sp1_ClearRect(&clip, 0x00, 0, SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
```

### Flags de inicialización
- `SP1_IFLAG_MAKE_ROTTBL`: genera tabla de rotaciones software.
- `SP1_IFLAG_OVERWRITE_TILES`: sobrescribe tabla de tiles.
- `SP1_IFLAG_OVERWRITE_DFILE`: sobrescribe pantalla durante inicialización.

**Ejemplo:**
```c
sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL, 0x00, 0);
```

---

## Referencia de defines y estructuras

Sección de referencia con **defines**, **flags**, **funciones de dibujo** y **estructuras**.

### Defines y constantes

**Tipos y atributos de sprites**

- `SP1_TYPE_OCCLUDE` (0x80) — background and sprites underneath will not be drawn
- `SP1_TYPE_BGNDCLR` (0x10) — for occluding sprites, copy background tile into pixel buffer before draw
- `SP1_TYPE_2BYTE` (0x40) — sprite graphic consists of (mask,graph) pairs (uso en `sp1_CreateSpr`)
- `SP1_TYPE_1BYTE` (0x00) — sprite graphic consists of graph only (uso en `sp1_CreateSpr`)

**Máscaras de atributo**

- `SP1_AMASK_TRANS` (0xff) — attribute mask for a transparent-colour sprite
- `SP1_AMASK_INK` (0xf8) — attribute mask for an ink-only sprite
- `SP1_AMASK_PAPER` (0xc7) — attribute mask for a paper-only sprite
- `SP1_AMASK_NOFLASH` (0x7f) — attribute mask for no-flash
- `SP1_AMASK_NOBRIGHT` (0xbf) — attribute mask for no-bright
- `SP1_ATTR_TRANS` (0x00) — attribute for a transparent-colour sprite

**Flags de rectángulo y printing**

- `SP1_RFLAG_TILE` (0x01) — controla limpieza/actualización de tiles
- `SP1_RFLAG_COLOUR` (0x02) — controla limpieza/actualización de atributos (colores)
- `SP1_RFLAG_SPRITE` (0x04) — controla limpieza/actualización de sprites

- `SP1_PSSFLAG_INVALIDATE` (0x01)
- `SP1_PSSFLAG_XWRAP` (0x02)
- `SP1_PSSFLAG_YINC` (0x04)
- `SP1_PSSFLAG_YWRAP` (0x08)

**Flags de inicialización (updater)**

- `SP1_IFLAG_MAKE_ROTTBL` (0x01) — genera tabla de rotaciones software
- `SP1_IFLAG_OVERWRITE_TILES` (0x02) — sobrescribe tabla de tiles
- `SP1_IFLAG_OVERWRITE_DFILE` (0x04) — sobrescribe pantalla durante inicialización

### Funciones de dibujo (draw functions)

Las funciones de dibujo se pasan como puntero al crear sprites o inicializar `sp1_cs`.

- `SP1_DRAW_MASK2()` — masked sprite 2-byte definition (mask,graph) pairs; rotación por software (usa MASK2_NR si no hay rotación)
- `SP1_DRAW_MASK2NR()` — igual que MASK2 pero sin rotación
- `SP1_DRAW_MASK2LB()` / `SP1_DRAW_MASK2RB()` — versiones para bordes izquierdo/derecho

- `SP1_DRAW_LOAD2()` / `SP1_DRAW_LOAD2NR()` / `SP1_DRAW_LOAD2LB()` / `SP1_DRAW_LOAD2RB()` — carga 2-byte, máscara ignorada
- `SP1_DRAW_OR2()` / `SP1_DRAW_OR2NR()` / `SP1_DRAW_OR2LB()` / `SP1_DRAW_OR2RB()` — OR 2-byte
- `SP1_DRAW_XOR2()` / `SP1_DRAW_XOR2NR()` / `SP1_DRAW_XOR2LB()` / `SP1_DRAW_XOR2RB()` — XOR 2-byte
- `SP1_DRAW_LOAD2LBIM()` / `SP1_DRAW_LOAD2RBIM()` — LOAD 2-byte left/right con máscara implícita

- `SP1_DRAW_LOAD1()` / `SP1_DRAW_LOAD1NR()` / `SP1_DRAW_LOAD1LB()` / `SP1_DRAW_LOAD1RB()` — load 1-byte (sin máscara)
- `SP1_DRAW_OR1()` / `SP1_DRAW_OR1NR()` / `SP1_DRAW_OR1LB()` / `SP1_DRAW_OR1RB()` — OR 1-byte
- `SP1_DRAW_XOR1()` / `SP1_DRAW_XOR1NR()` / `SP1_DRAW_XOR1LB()` / `SP1_DRAW_XOR1RB()` — XOR 1-byte
- `SP1_DRAW_LOAD1LBIM()` / `SP1_DRAW_LOAD1RBIM()` — LOAD 1-byte left/right con máscara implícita

- `SP1_DRAW_ATTR()` — no dibuja pixels, solo atributos

### Estructuras (campos y descripción)

#### `struct sp1_Rect`
Rectángulo en coordenadas de tiles.
- `uint8_t row` — fila (tile) de origen
- `uint8_t col` — columna (tile) de origen
- `uint8_t width` — ancho en tiles
- `uint8_t height` — alto en tiles

#### `struct sp1_update` (10 bytes)
Describe cada tile gestionado por SP1.
- `uint8_t nload` — +0: bit7=1 invalidado, bit6=1 removido, bits5:0 = número de sprites oclusivos presentes + 1
- `uint8_t colour` — +1: atributo de fondo (colour/attribute)
- `uint16_t tile` — +2: código 16-bit de tile (si MSB != 0 se trata como dirección de gráfico, sino lookup en tabla)
- `struct sp1_cs *slist` — +4: lista de sprites ocupando este tile (BIG ENDIAN; MSB=0 si none); apunta a `sp1_cs.attr_mask`
- `struct sp1_update *ulist` — +6: siguiente update struct en la lista de draw (BIG ENDIAN)
- `uint8_t *screen` — +8: dirección en el fichero de pantalla donde se dibuja este tile

#### `struct sp1_ss` (20 bytes)
Estructura principal del sprite.
- `uint8_t row` — +0: coordenada Y en tiles actual
- `uint8_t col` — +1: coordenada X en tiles actual
- `uint8_t width` — +2: ancho en tiles
- `uint8_t height` — +3: alto en tiles
- `uint8_t vrot` — +4: bit7=1 para definición gráfica 2-byte; bits2:0 = rotación vertical actual (0..7)
- `uint8_t hrot` — +5: rotación horizontal actual (0..7)
- `uint8_t *frame` — +6: dirección del frame actual (se suma a pointers de gráficos)
- `uint8_t res0` — +8: opcode reservado ("LD A,n")
- `uint8_t e_hrot` — +9: horizontal rotation effective (MSB de tabla de rotaciones)
- `uint8_t res1` — +10: opcode reservado ("LD BC,nn")
- `uint16_t e_offset` — +11: offset efectivo a sumar a pointers de gráficos (resultado rotación vertical + frame addr)
- `uint8_t res2` — +13: opcode reservado
- `uint8_t res3` — +14: opcode reservado
- `struct sp1_cs *first` — +15: primer `sp1_cs` de este sprite (BIG ENDIAN)
- `uint8_t xthresh` — +17: umbral horizontal de píxeles para dibujar última columna (1 por defecto)
- `uint8_t ythresh` — +18: umbral vertical para dibujar última fila (1 por defecto)
- `uint8_t nactive` — +19: número de `sp1_cs` actualmente en pantalla (escrito por sp1_MoveSpr*)

#### `struct sp1_cs` (24 bytes)
Fragmento de sprite que cabe en un tile.
- `struct sp1_cs *next_in_spr` — +0: siguiente char dentro del mismo sprite (row-major)
- `struct sp1_update *update` — +2: update struct del tile que ocupa actualmente
- `uint8_t plane` — +4: plano que ocupa (0 = más cercano al viewer)
- `uint8_t type` — +5: bit7=occluding, bit6=última columna, bit5=última fila, bit4=clear pixelbuffer
- `uint8_t attr_mask` — +6: máscara de atributos (AND con underlying attribute), default 0xff transparente
- `uint8_t attr` — +7: color del sprite (OR con underlying attribute), default 0
- `void *ss_draw` — +8: apunta a código embebido en `struct sp1_ss` (ss + 8)
- `uint8_t res0` — +10: opcode reservado ("LD HL,nn")
- `uint8_t *def` — +11: puntero a definición gráfica
- `uint8_t res1` — +13: opcode reservado ("LD IX,nn")
- `uint8_t res2` — +14: reservado
- `uint8_t *l_def` — +15: definición gráfica para el char a la izquierda
- `uint8_t res3` — +17: opcode reservado ("CALL nn")
- `void *draw` — +18: puntero a la función draw para este char
- `struct sp1_cs *next_in_upd` — +20: siguiente sprite ocupando la misma tile (en la lista del update)
- `struct sp1_cs *prev_in_upd` — +22: anterior sprite ocupando la misma tile

#### `struct sp1_ap` (2 bytes)
Par atributo/máscara para colores de sprite.
- `uint8_t attr_mask` — máscara de atributo (AND), default 0xff
- `uint8_t attr` — atributo (OR), default 0

#### `struct sp1_tp` (3 bytes)
Par tile/atributo de fondo.
- `uint8_t attr` — colour
- `uint16_t tile` — código de tile

#### `struct sp1_pss` (11 bytes)
Estructura de estado de impresión de cadenas.
- `struct sp1_Rect *bounds` — rectángulo límite
- `uint8_t flags` — flags (invalidate/xwrap/yinc/ywrap)
- `uint8_t x` — cursor x (dentro de `bounds`)
- `uint8_t y` — cursor y (dentro de `bounds`)
- `uint8_t attr_mask` — máscara actual
- `uint8_t attr` — atributo actual
- `struct sp1_update *pos` — reservado: update asociado a coordenadas del cursor
- `void *visit` — función visit opcional: `void (*visit)(struct sp1_update *)`

---

## Prototypes públicos (funciones)

Referencia rápida de las funciones de **sp1** agrupadas por funcionalidad.

| Función | Grupo | Descripción |
|---|---:|---|
| [sp1_CreateSpr](#sp1_createspr) | Sprites | Crea y devuelve `struct sp1_ss *` |
| [sp1_AddColSpr](#sp1_addcolspr) | Sprites | Añade una columna al sprite |
| [sp1_ChangeSprType](#sp1_changesprtype) | Sprites | Cambia el drawf de un `sp1_cs` |
| [sp1_DeleteSpr](#sp1_deletespr) | Sprites | Elimina y libera un sprite |
| [sp1_MoveSprAbs](#sp1_movesprabs) | Movimiento | Mueve sprite a coordenadas absolutas |
| [sp1_MoveSprRel](#sp1_movesprrel) | Movimiento | Mueve sprite de forma relativa |
| [sp1_MoveSprPix](#sp1_movesprpix) | Movimiento | Mueve sprite por píxeles (x,y) |
| [sp1_IterateSprChar](#sp1_iteratesprchar) | Iteración | Recorre `sp1_cs` de un sprite |
| [sp1_IterateUpdateSpr](#sp1_iterateupdatespr) | Iteración | Recorre updates ocupados por sprite |
| [sp1_GetSprClrAddr](#sp1_getsprclraddr) | Color buffer | Obtiene dirección del buffer de colores |
| [sp1_PutSprClr](#sp1_putsprclr) | Color buffer | Escribe pares `sp1_ap` en buffer |
| [sp1_GetSprClr](#sp1_getsprclr) | Color buffer | Lee pares `sp1_ap` desde buffer |
| [sp1_PreShiftSpr](#sp1_preshiftspr) | Utilidades | Precalcula versiones desplazadas |
| [sp1_InitCharStruct](#sp1_initcharstruct) | Char struct | Inicializa `sp1_cs` independiente |
| [sp1_InsertCharStruct](#sp1_insertcharstruct) | Char struct | Inserta `sp1_cs` en `sp1_update` |
| [sp1_RemoveCharStruct](#sp1_removecharstruct) | Char struct | Elimina `sp1_cs` |
| [sp1_TileEntry](#sp1_tileentry) | Tiles | Define o asigna un tile |
| [sp1_PrintAt](#sp1_printat) | Tiles | Imprime tile sin invalidar |
| [sp1_PrintAtInv](#sp1_printatinv) | Tiles | Imprime e invalida |
| [sp1_ScreenStr](#sp1_screenstr) | Tiles | Lee código de tile en pantalla |
| [sp1_ScreenAttr](#sp1_screenattr) | Tiles | Lee atributo en pantalla |
| [sp1_PrintString](#sp1_printstring) | Text | Imprime cadena con `sp1_pss` |
| [sp1_SetPrintPos](#sp1_setprintpos) | Text | Mueve cursor de impresión |
| [sp1_GetTiles](#sp1_gettiles) | Tiles | Copia tiles a buffer `sp1_tp` |
| [sp1_PutTiles](#sp1_puttiles) | Tiles | Restaura tiles desde buffer |
| [sp1_PutTilesInv](#sp1_puttilesinv) | Tiles | Restaura e invalida |
| [sp1_ClearRect](#sp1_clearrect) | Tiles | Limpia rectángulo sin invalidar |
| [sp1_ClearRectInv](#sp1_clearrectinv) | Tiles | Limpia e invalida rectángulo |
| [sp1_Initialize](#sp1_initialize) | Updater | Inicializa SP1 (flags `SP1_IFLAG_*`) |
| [sp1_UpdateNow](#sp1_updatenow) | Updater | Fuerza refresco inmediato |
| [sp1_GetUpdateStruct](#sp1_getupdatestruct) | Updater | Obtiene `sp1_update` por coordenada |
| [sp1_IterateUpdateArr](#sp1_iterateupdatearr) | Updater | Itera array de `sp1_update*` terminado en 0 |
| [sp1_IterateUpdateRect](#sp1_iterateupdaterect) | Updater | Itera updates dentro de rect |
| [sp1_InvUpdateStruct](#sp1_invupdatestruct) | Updater | Marca update como inválido |
| [sp1_ValUpdateStruct](#sp1_valupdatestruct) | Updater | Marca update como válido |
| [sp1_DrawUpdateStructIfInv](#sp1_drawupdatestructifinv) | Updater | Dibuja si inválido |
| [sp1_DrawUpdateStructIfVal](#sp1_drawupdatestructifval) | Updater | Dibuja si válido |
| [sp1_DrawUpdateStructIfNotRem](#sp1_drawupdatestructifnotrem) | Updater | Dibuja si no marcado como removido |
| [sp1_DrawUpdateStructAlways](#sp1_drawupdatestructalways) | Updater | Dibuja siempre |
| [sp1_RemoveUpdateStruct](#sp1_removeupdatestruct) | Updater | Marca update como removido |
| [sp1_RestoreUpdateStruct](#sp1_restoreupdatestruct) | Updater | Restaura update marcado como removido |
| [sp1_Invalidate](#sp1_invalidate) | Updater | Invalida un rectángulo entero |
| [sp1_Validate](#sp1_validate) | Updater | Valida un rectángulo entero |


---

## Flujo de inicialización recomendado
Flujo típico para arrancar SP1 en un proyecto:

1) Inicializar la librería con flags y fondo.
2) Definir tiles básicos con `sp1_TileEntry` si usas tabla de tiles.
3) Crear sprites y asignar frames.
4) Posicionar sprites con `sp1_MoveSprAbs` o `sp1_MoveSprPix`.
5) En el bucle principal, invalidar áreas y llamar a `sp1_UpdateNow`.

**Ejemplo:**
```c
sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL, 0x00, 0);
sp1_TileEntry('A', tileA);
struct sp1_ss *spr = sp1_CreateSpr(SP1_DRAW_MASK2, SP1_TYPE_2BYTE, 2, (int)gfx, 0);
sp1_MoveSprAbs(spr, &clip, frame0, 8, 12, 0, 0);
// loop:
// sp1_UpdateNow();
```

---

## Sprites: creación y configuración

### sp1_CreateSpr
Crea un sprite principal y devuelve un puntero a `struct sp1_ss`. Define tipo de gráficos (1 o 2 bytes), altura en tiles y plano.

> `struct sp1_ss *sp1_CreateSpr(void *drawf, uint16_t type, uint16_t height, int graphic, uint16_t plane)`

**Ejemplo:**
```c
struct sp1_ss *spr = sp1_CreateSpr(SP1_DRAW_MASK2, SP1_TYPE_2BYTE, 2, (int)gfx, 0);
```
- Nota: `type` combina `SP1_TYPE_2BYTE`/`_1BYTE` y flags como `SP1_TYPE_OCCLUDE`. El `height` es en tiles (p. ej. 2 para 16px si cada tile 8px).

### sp1_AddColSpr
Añade una columna (tile column) al sprite ya creado. Útil para sprites más anchos que 1 tile.
> `uint16_t sp1_AddColSpr(struct sp1_ss *s, void *drawf, uint16_t type, int graphic, uint16_t plane)`

**Ejemplo:**
```c
sp1_AddColSpr(spr, SP1_DRAW_MASK2, SP1_TYPE_2BYTE, (int)gfx_col2, 0);
```
- Nota: añade columnas para sprites anchos; devuelve offset de columna y se suele llamar justo después de `sp1_CreateSpr`.

### sp1_ChangeSprType
Cambia la función de dibujo de un `struct sp1_cs` (sprite char) existente.
> `void sp1_ChangeSprType(struct sp1_cs *c, void *drawf)`

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_XOR2);
```
- Nota: cambiar tipo afecta solo a ese `sp1_cs` (útil para bordes o efectos especiales).

### sp1_DeleteSpr
Libera y elimina un sprite creado con `sp1_CreateSpr`.
> `void sp1_DeleteSpr(struct sp1_ss *s)`

**Ejemplo:**
```c
sp1_DeleteSpr(spr);
```
- Nota: llamar solo cuando el sprite ya no esté en pantalla; libera recursos internos.

---

## Sprites: movimiento

### sp1_MoveSprAbs
Mueve el sprite a coordenadas absolutas (tile + rotación). Requiere que el sprite no esté en pantalla.
> `void sp1_MoveSprAbs(struct sp1_ss *s, struct sp1_Rect *clip, void *frame, uint16_t row, uint16_t col, uint16_t vrot, uint16_t hrot)`

**Ejemplo:**
```c
// (sprite, clip, frame, Y, X, rotY, rotX)
sp1_MoveSprAbs(spr, &clip, frame0, 8, 12, 0, 0);
```
- Nota: debe llamarse con el sprite fuera de pantalla para reposicionar de forma segura; `row/col` en tiles.

### sp1_MoveSprRel
Mueve el sprite en relativo (delta en tiles y rotaciones).
> `void sp1_MoveSprRel(struct sp1_ss *s, struct sp1_Rect *clip, void *frame, int rel_row, int rel_col, int rel_vrot, int rel_hrot)`

**Ejemplo:**
```c
sp1_MoveSprRel(spr, &clip, frame0, 0, 1, 0, 0); // mover 1 tile a la derecha
```
- Nota: delta en tiles; ideal para movimientos discretos (1 tile = 8px).

### sp1_MoveSprPix
Mueve el sprite por píxeles (x,y). Internamente ajusta tile y rotaciones.
> `void sp1_MoveSprPix(struct sp1_ss *s, struct sp1_Rect *clip, void *frame, uint16_t x, uint16_t y)`

**Ejemplo:**
```c
sp1_MoveSprPix(spr, &clip, frame0, 96, 40);
```
- Nota: coordenadas en píxeles; SP1 ajusta tile y rotaciones internamente.

---

## Sprites: iteración y utilidades

### sp1_IterateSprChar
Recorre los `struct sp1_cs` de un sprite y llama a un hook por cada uno. Hook típico: `void hook(uint count, struct sp1_cs *c)`.
> `void sp1_IterateSprChar(struct sp1_ss *s, void *hook)`

**Ejemplo:**
```c
void my_hook(uint count, struct sp1_cs *c) {
    (void)count; c->attr = 0x47;
}
sp1_IterateSprChar(spr, (void*)my_hook);
```
- Nota: el hook puede cambiar `attr`/`attr_mask` para cada `sp1_cs`.

### sp1_IterateUpdateSpr
Itera las celdas del sprite que están en pantalla y llama a un hook por `sp1_update`. Hook típico: `void hook(uint count, struct sp1_update *u)`.
> `void sp1_IterateUpdateSpr(struct sp1_ss *s, void *hook)`

**Ejemplo:**
```c
void my_upd_hook(uint count, struct sp1_update *u) {
    (void)count; u->colour = 0x1F;
}
sp1_IterateUpdateSpr(spr, (void*)my_upd_hook);
```
- Nota: hook recibe `sp1_update*`, útil para operar sobre tiles afectados.

### sp1_GetSprClrAddr
Obtiene la dirección interna donde están los atributos (colores) del sprite.
> `void sp1_GetSprClrAddr(struct sp1_ss *s, uint8_t **sprdest)`

**Ejemplo:**
```c
uint8_t *sprclr;
sp1_GetSprClrAddr(spr, &sprclr);
```
- Nota: `sp1_GetSprClrAddr` devuelve un puntero a buffer interno de atributos; modificar con `sp1_PutSprClr`/`sp1_GetSprClr`.

### sp1_PutSprClr
Escribe pares atributo/máscara (`struct sp1_ap`) sobre el buffer de colores del sprite.

**Ejemplo:**
```c
struct sp1_ap colors[2] = {{0xFF, 0x47}, {0xFF, 0x43}};
uint8_t *sprclr;
sp1_GetSprClrAddr(spr, &sprclr);
sp1_PutSprClr(&sprclr, colors, 2);
```

### sp1_GetSprClr
Lee pares atributo/máscara desde el buffer de colores del sprite.
> `void sp1_GetSprClr(uint8_t **sprsrc, struct sp1_ap *dest, uint16_t n)`

**Ejemplo:**
```c
struct sp1_ap out[2];
uint8_t *sprclr;
sp1_GetSprClrAddr(spr, &sprclr);
sp1_GetSprClr(&sprclr, out, 2);
```

### sp1_PreShiftSpr
Precalcula versiones desplazadas de un frame para acelerar rotaciones por software.
> `void *sp1_PreShiftSpr(uint16_t flag, uint16_t height, uint16_t width, void *srcframe, void *destframe, uint16_t rshift)`

**Ejemplo:**
```c
void *shifted = sp1_PreShiftSpr(0, 2, 2, srcframe, dstframe, 3);
```
- Nota: genera versiones desplazadas (rshift) para optimizar rotaciones por software.

---

## Char structs independientes

### sp1_InitCharStruct
Inicializa un `struct sp1_cs` para usarlo de forma independiente (no conectado a un sprite).
> `void sp1_InitCharStruct(struct sp1_cs *cs, void *drawf, uint16_t type, void *graphic, uint16_t plane)`

**Ejemplo:**
```c
sp1_InitCharStruct(&cs, SP1_DRAW_OR1, SP1_TYPE_1BYTE, gfx_tile, 0);
```
- Nota: para dibujar char sueltos; si usas funciones NR, inicializa `l_def` manualmente.

### sp1_InsertCharStruct
Inserta el `struct sp1_cs` en un `struct sp1_update` para que se dibuje en pantalla.
> `void sp1_InsertCharStruct(struct sp1_update *u, struct sp1_cs *cs)`

**Ejemplo:**
```c
struct sp1_update *u = sp1_GetUpdateStruct(10, 10);
sp1_InsertCharStruct(u, &cs);
```
- Nota: se insertará en la próxima actualización si el update está inválido o se invalida.

### sp1_RemoveCharStruct
Elimina el `struct sp1_cs` de la pantalla.
> `void sp1_RemoveCharStruct(struct sp1_cs *cs)`

**Ejemplo:**
```c
sp1_RemoveCharStruct(&cs);
```
- Nota: elimina y quita enlaces; no reutilices `cs` sin re-inicializarlo.

---

## Tiles y texto

### sp1_TileEntry
Define o asigna un tile en la tabla de tiles.
> `void *sp1_TileEntry(uint16_t c, void *def)`

**Ejemplo:**
```c
sp1_TileEntry('A', tileA);
```
- Nota: guardar retorno si deseas restaurar la entrada anterior.

### sp1_PrintAt
Imprime un tile con atributo en una posición de pantalla.

> `void sp1_PrintAt(uint16_t row, uint16_t col, uint16_t colour, uint16_t tile)`

**Ejemplo:**
```c
sp1_PrintAt(5, 10, 0x47, 'A');
```

- Nota: `PrintAtInv` invalida la celda para refresco inmediato.

### sp1_PrintAtInv
Igual que `sp1_PrintAt` pero invalidando el área para refresco inmediato.

> `void sp1_PrintAtInv(uint16_t row, uint16_t col, uint16_t colour, uint16_t tile)`

**Ejemplo:**
```c
sp1_PrintAtInv(5, 11, 0x47, 'B');
```

### sp1_ScreenStr
Devuelve el tile actual en una posición (como código de pantalla).

> `uint16_t sp1_ScreenStr(uint16_t row, uint16_t col)`
- Nota: lecturas directas del buffer gestionado por SP1.

**Ejemplo:**
```c
uint16_t t = sp1_ScreenStr(5, 10);
```

### sp1_ScreenAttr
Devuelve el atributo de color actual en una posición.

> `uint16_t sp1_ScreenAttr(uint16_t row, uint16_t col)`

**Ejemplo:**
```c
uint16_t a = sp1_ScreenAttr(5, 10);
```

### sp1_PrintString
Imprime una cadena dentro de los límites definidos en `sp1_pss`.
> `void sp1_PrintString(struct sp1_pss *ps, uint8_t *s)`

**Ejemplo:**
```c
struct sp1_pss ps = { &clip, SP1_PSSFLAG_INVALIDATE, 0, 0, 0xFF, 0x47, 0, 0 };
sp1_PrintString(&ps, (uint8_t*)"HOLA");
```
- Nota: usar `SP1_PSSFLAG_INVALIDATE` para que las celdas escritas se refresquen.

### sp1_SetPrintPos
Mueve el cursor de impresión dentro de un `sp1_pss`.
> `void sp1_SetPrintPos(struct sp1_pss *ps, uint16_t row, uint16_t col)`

**Ejemplo:**
```c
sp1_SetPrintPos(&ps, 2, 4);
```

### sp1_GetTiles
Copia tiles y atributos de un rectángulo a un buffer (`sp1_tp`).
> `void sp1_GetTiles(struct sp1_Rect *r, struct sp1_tp *dest)`

**Ejemplo:**
```c
struct sp1_tp saved[16];
sp1_GetTiles(&clip, saved);
```
- Nota: útil para salvar/ restaurar background donde vas a dibujar sprites.

### sp1_PutTiles
Restaura tiles y atributos desde un buffer (`sp1_tp`).
> `void sp1_PutTiles(struct sp1_Rect *r, struct sp1_tp *src)`

**Ejemplo:**
```c
sp1_PutTiles(&clip, saved);
```

### sp1_PutTilesInv
Restaura tiles y atributos e invalida el rectángulo para redibujado.
> `void sp1_PutTilesInv(struct sp1_Rect *r, struct sp1_tp *src)`

**Ejemplo:**
```c
sp1_PutTilesInv(&clip, saved);
```

### sp1_ClearRect
Rellena un rectángulo con color/tile opcional y flags de qué limpiar.
> `void sp1_ClearRect(struct sp1_Rect *r, uint16_t colour, uint16_t tile, uint16_t rflag)`

**Ejemplo:**
```c
sp1_ClearRect(&clip, 0x00, 0, SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
```
- Nota: `rflag` controla qué se limpia (tile/colour/sprite).

### sp1_ClearRectInv
Igual que `sp1_ClearRect` pero invalidando el rectángulo.
> `void sp1_ClearRectInv(struct sp1_Rect *r, uint16_t colour, uint16_t tile, uint16_t rflag)`

**Ejemplo:**
```c
sp1_ClearRectInv(&clip, 0x00, 0, SP1_RFLAG_TILE | SP1_RFLAG_COLOUR);
```

---

## Updater (actualización de pantalla)

### sp1_Initialize
Inicializa SP1, configurando flags y tile/color de fondo. El parametro `tile` sera usado para rellenar la pantalla.
> `void sp1_Initialize(uint16_t iflag, uint16_t colour, uint16_t tile)`

**Ejemplo:**
```c
sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL, 0x00, 0);
```

- Nota: pasar `SP1_IFLAG_MAKE_ROTTBL` si necesitas rotaciones por software.

### sp1_UpdateNow
Fuerza el refresco inmediato de las áreas invalidadas.
> `void sp1_UpdateNow(void)`

**Ejemplo:**
```c
sp1_UpdateNow();
```
- Nota: llama cada frame para procesar la lista de celdas invalidadas.

### sp1_GetUpdateStruct
Obtiene el `sp1_update` asociado a una coordenada tile.
> `struct sp1_update *sp1_GetUpdateStruct(uint16_t row, uint16_t col)`

**Ejemplo:**
```c
struct sp1_update *u = sp1_GetUpdateStruct(10, 5);
```

- Nota: no modificar campos internos sin entender la lógica del updater.

### sp1_IterateUpdateArr
Itera un array de `sp1_update*` terminado en 0 y llama un hook.
> `void sp1_IterateUpdateArr(struct sp1_update **ua, void *hook)`

**Ejemplo:**
```c
struct sp1_update *list[] = { u, 0 };
sp1_IterateUpdateArr(list, (void*)my_upd_hook);
```
- Nota: el array debe estar terminado en NULL (0).

### sp1_IterateUpdateRect
Itera todos los `sp1_update` dentro de un rectángulo.
> `void sp1_IterateUpdateRect(struct sp1_Rect *r, void *hook)`

**Ejemplo:**
```c
sp1_IterateUpdateRect(&clip, (void*)my_upd_hook);
```

### sp1_InvUpdateStruct
Marca un `sp1_update` como inválido (necesita redibujo).
> `void sp1_InvUpdateStruct(struct sp1_update *u)`

**Ejemplo:**
```c
sp1_InvUpdateStruct(u);
```

### sp1_ValUpdateStruct
Marca un `sp1_update` como válido (no necesita redibujo).
> `void sp1_ValUpdateStruct(struct sp1_update *u)`

**Ejemplo:**
```c
sp1_ValUpdateStruct(u);
```

### sp1_DrawUpdateStructIfInv
Dibuja el `sp1_update` solo si está inválido.
> `void sp1_DrawUpdateStructIfInv(struct sp1_update *u)`

**Ejemplo:**
```c
sp1_DrawUpdateStructIfInv(u);
```

- Nota: usar esta funcion para drawing selectivo en hooks/depuración.

### sp1_DrawUpdateStructIfVal
Dibuja el `sp1_update` solo si está válido.
> `void sp1_DrawUpdateStructIfVal(struct sp1_update *u)`

**Ejemplo:**
```c
sp1_DrawUpdateStructIfVal(u);
```
- Nota: usar esta funcion para drawing selectivo en hooks/depuración.

### sp1_DrawUpdateStructIfNotRem
Dibuja el `sp1_update` solo si no está marcado como eliminado.
> `void sp1_DrawUpdateStructIfNotRem(struct sp1_update *u)`

**Ejemplo:**
```c
sp1_DrawUpdateStructIfNotRem(u);
```

- Nota: usar esta funcion para drawing selectivo en hooks/depuración.

### sp1_DrawUpdateStructAlways
Dibuja el `sp1_update` siempre, sin condiciones.
> `void sp1_DrawUpdateStructAlways(struct sp1_update *u)`

**Ejemplo:**
```c
sp1_DrawUpdateStructAlways(u);
```

- Nota: usar esta funcion para drawing selectivo en hooks/depuración.

### sp1_RemoveUpdateStruct
Marca un `sp1_update` como removido para que no se dibuje.
> `void sp1_RemoveUpdateStruct(struct sp1_update *u)`

**Ejemplo:**
```c
sp1_RemoveUpdateStruct(u);
```
- Nota: útil para elementos temporales que no deben dibujarse.

### sp1_RestoreUpdateStruct
Restaura un `sp1_update` previamente removido.

> `void sp1_RestoreUpdateStruct(struct sp1_update *u)`

**Ejemplo:**
```c
sp1_RestoreUpdateStruct(u);
```

### sp1_Invalidate
 Invalida todos los `sp1_update` en un rectángulo para control fino del redraw.

> `void sp1_Invalidate(struct sp1_Rect *r)`

**Ejemplo:**
```c
sp1_Invalidate(&clip);
```

### sp1_Validate
 Valida todos los `sp1_update` en un rectángulo para control fino del redraw.

> `void sp1_Validate(struct sp1_Rect *r)`

**Ejemplo:**
```c
sp1_Validate(&clip);
```

---

## Funciones de dibujo (draw functions)

Estas funciones se pasan como puntero al crear sprites o inicializar `sp1_cs`.

### SP1_DRAW_MASK2
Dibujo con máscara y gráfico (2 bytes por columna). Rotación software.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_MASK2, SP1_TYPE_2BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_MASK2NR
Como `SP1_DRAW_MASK2` pero sin rotación (tile boundary exacto).

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_MASK2NR, SP1_TYPE_2BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_MASK2LB
Como `SP1_DRAW_MASK2` pero para el borde izquierdo del sprite.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_MASK2LB);
```

### SP1_DRAW_MASK2RB
Como `SP1_DRAW_MASK2` pero para el borde derecho del sprite.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_MASK2RB);
```

### SP1_DRAW_LOAD2
Carga gráfico (2 bytes), ignora máscara. Rotación software.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_LOAD2, SP1_TYPE_2BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_LOAD2NR
Como `SP1_DRAW_LOAD2` sin rotación.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_LOAD2NR, SP1_TYPE_2BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_LOAD2LB
Como `SP1_DRAW_LOAD2` para borde izquierdo.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_LOAD2LB);
```

### SP1_DRAW_LOAD2RB
Como `SP1_DRAW_LOAD2` para borde derecho.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_LOAD2RB);
```

### SP1_DRAW_OR2
Combina gráfico por OR (2 bytes). Rotación software.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_OR2, SP1_TYPE_2BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_OR2NR
OR 2 bytes sin rotación.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_OR2NR, SP1_TYPE_2BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_OR2LB
OR 2 bytes borde izquierdo.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_OR2LB);
```

### SP1_DRAW_OR2RB
OR 2 bytes borde derecho.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_OR2RB);
```

### SP1_DRAW_XOR2
XOR 2 bytes con rotación.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_XOR2, SP1_TYPE_2BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_XOR2NR
XOR 2 bytes sin rotación.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_XOR2NR, SP1_TYPE_2BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_XOR2LB
XOR 2 bytes borde izquierdo.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_XOR2LB);
```

### SP1_DRAW_XOR2RB
XOR 2 bytes borde derecho.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_XOR2RB);
```

### SP1_DRAW_LOAD2LBIM
LOAD 2 bytes borde izquierdo con máscara implícita.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_LOAD2LBIM);
```

### SP1_DRAW_LOAD2RBIM
LOAD 2 bytes borde derecho con máscara implícita.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_LOAD2RBIM);
```

### SP1_DRAW_LOAD1
Carga gráfico de 1 byte (sin máscara). Rotación software.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_LOAD1, SP1_TYPE_1BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_LOAD1NR
LOAD 1 byte sin rotación.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_LOAD1NR, SP1_TYPE_1BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_LOAD1LB
LOAD 1 byte borde izquierdo.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_LOAD1LB);
```

### SP1_DRAW_LOAD1RB
LOAD 1 byte borde derecho.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_LOAD1RB);
```

### SP1_DRAW_OR1
OR 1 byte con rotación.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_OR1, SP1_TYPE_1BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_OR1NR
OR 1 byte sin rotación.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_OR1NR, SP1_TYPE_1BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_OR1LB
OR 1 byte borde izquierdo.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_OR1LB);
```

### SP1_DRAW_OR1RB
OR 1 byte borde derecho.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_OR1RB);
```

### SP1_DRAW_XOR1
XOR 1 byte con rotación.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_XOR1, SP1_TYPE_1BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_XOR1NR
XOR 1 byte sin rotación.

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_XOR1NR, SP1_TYPE_1BYTE, 2, (int)gfx, 0);
```

### SP1_DRAW_XOR1LB
XOR 1 byte borde izquierdo.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_XOR1LB);
```

### SP1_DRAW_XOR1RB
XOR 1 byte borde derecho.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_XOR1RB);
```

### SP1_DRAW_LOAD1LBIM
LOAD 1 byte borde izquierdo con máscara implícita.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_LOAD1LBIM);
```

### SP1_DRAW_LOAD1RBIM
LOAD 1 byte borde derecho con máscara implícita.

**Ejemplo:**
```c
sp1_ChangeSprType(cs, SP1_DRAW_LOAD1RBIM);
```

### SP1_DRAW_ATTR
Dibuja solo atributos (sin píxeles).

**Ejemplo:**
```c
sp1_CreateSpr(SP1_DRAW_ATTR, SP1_TYPE_1BYTE, 2, (int)gfx, 0);
```
