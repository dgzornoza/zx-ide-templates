#ifndef __CORE_UTILS_DRAW_MAP_REGION_H__
#define __CORE_UTILS_DRAW_MAP_REGION_H__

#include <stdint.h>

/**
 * Configuration structure for drawing a tiled map region.
 * Set the fields in draw_map_config before calling draw_map_region().
 *
 * Tile graphics and attributes are NOT carried in this struct: draw_map_region()
 * reads them from the shared SP1 tile buffer (tile_gfx / tile_attr, declared in
 * graphics.h). The caller is responsible for pre-registering each SP1 slot used
 * by the map via graphics_register_tile_compressed_range() (or equivalent).
 */
typedef struct
{
    /** Pointer to the tile index map exported from Tiled (row-major, 1-based indices; 0 = empty). */
    const uint8_t *tiles_map;
    /** Width of the map region in tiles. */
    uint8_t map_width;
    /** Height of the map region in tiles. */
    uint8_t map_height;
    /** Screen column (in characters) where the left edge of the map is drawn. */
    uint8_t map_x_offset;
    /** Screen row (in characters) where the top edge of the map is drawn. */
    uint8_t map_y_offset;
    /**
     * Base tile-slot index used by draw_map_region() to look up each tile in
     * the shared tile buffer.
     */
    uint8_t start_tile_entry_index;
} DrawMapRegionConfig;

/**
 * Global configuration used by draw_map_region().
 * Populate this struct before calling draw_map_region().
 */
extern DrawMapRegionConfig draw_map_config;

/**
 * Draws a Tiled-exported tile map region against the shared tile buffer.
 * For each non-zero map cell, computes tile_slot = start_tile_entry_index +
 * (map_tile_index - 1) and reads the bitmap from tile_gfx[sp1_slot] and the
 * colour attribute from tile_attr[sp1_slot] (see graphics.h).
 *
 * PRECONDITION:
 * - Set the fields in draw_map_config before calling this function.
 * - Every slot in the range must already be registered (graphics_register_tile_compressed_range() or
 * equivalent) before calling this function.
 */
void draw_map_region(void);

#endif // __CORE_UTILS_DRAW_MAP_REGION_H__
