#ifndef __CORE_UTILS_DRAW_MAP_REGION_H__
#define __CORE_UTILS_DRAW_MAP_REGION_H__

#include <stdint.h>

/* Campos comunes:
   - tiles_map: puntero al mapa
   - tiles_data: gráficos
   - tiles_attributes: atributos
   - map_width/map_height: tamaño
   - map_x_offset/map_y_offset: desplazamiento
*/
#define COMMON_FIELDS                \
    const uint8_t *tiles_map;        \
    const uint8_t *tiles_data;       \
    const uint8_t *tiles_attributes; \
    uint8_t map_width;               \
    uint8_t map_height;              \
    uint8_t map_x_offset;            \
    uint8_t map_y_offset;

/**
 * Configuration structure for drawing a tiled map.
 */
typedef struct
{
    COMMON_FIELDS
} DrawMapRegionParams;

typedef struct
{
    COMMON_FIELDS
    // If non-zero, draws a horizontally mirrored copy of the map on specified position, inverting the tile graphics.
    uint8_t has_mirror;
} DrawMapStaticRegionParams;

/**
 * Draws a Tiled-exported tile map using SP1 tile data and attributes as static, with support for mirrored drawing.
 * This function uses a single Tile ID (255) as a "volatile" slot for static or mirrored tiles.
 * Note: Forcing an immediate tile update. If the tile is redrawn or a sprite passes over it, the tile will contain garbage or the current tile at position 255.
 * It should be used ONLY for regions that do not change, such as a HUD or walls that will not be modified and where nothing will overlap that tile.
 */
extern void draw_map_static_region(DrawMapStaticRegionParams *params) __z88dk_fastcall;

/**
 * Draws a Tiled-exported tile map using SP1 tile data and attributes.
 * This function draws a region of the screen using the data from a tile map.
 * The tiles will be stored at the index specified by the tile_index property, so if the function is invoked again with the same indices,
 * they will be overwritten. Therefore, during an invalidation, the last tile assigned to that index will be the one that gets set.
 * Because of this, the game map indices must be managed carefully to avoid overwriting them.
 * @param params Configuration parameters for drawing the map region.
 */
extern void draw_map_region(DrawMapRegionParams *params) __z88dk_fastcall;

#endif // __CORE_UTILS_DRAW_MAP_REGION_H__
