#ifndef __CORE_UTILS_DRAW_TILED_DATA_H__
#define __CORE_UTILS_DRAW_TILED_DATA_H__

#include <stdint.h>

/**
 * Configuration structure for drawing a tiled map.
 * Using a global struct is mandatory in Z80 architecture
 * to avoid passing many parameters via the stack.
 */
typedef struct
{
    // Pointer to the tile map data (row-major, exported from Tiled).
    const uint8_t *tile_map;
    // Pointer to the tile graphics data (all tiles concatenated).
    const uint8_t *tiles_data;
    // Pointer to the tile attributes array (one per tile).
    const uint8_t *tiles_attributes;
    // Width of the map in tiles.
    uint8_t map_width;
    // Height of the map in tiles.
    uint8_t map_height;
    // X offset (in tiles) where the map will be drawn on screen.
    uint8_t map_x_offset;
    // Y offset (in tiles) where the map will be drawn on screen.
    uint8_t map_y_offset;
    // If non-zero, draws a horizontally mirrored copy of the map on the opposite side of the screen, inverting the tile graphics.
    uint8_t has_mirror;
} DrawTiledConfig;

extern DrawTiledConfig draw_tiled_config;

/**
 * Draws a Tiled-exported tile map using SP1 tile data and attributes.
 *
 * It reads all its configuration from the global `draw_tiled_config`
 * variable. Ensure all fields are set before calling.
 *
 * @remark Only use this function for static maps that don't change often,
 * since this implementacion is optimized for simplicity and memory usage, not for performance.
 */
void draw_tiled_data(void) __z88dk_fastcall;

#endif // __CORE_UTILS_DRAW_TILED_DATA_H__
