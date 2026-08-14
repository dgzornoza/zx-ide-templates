#ifndef __CORE_UTILS_DRAW_MAP_REGION_H__
#define __CORE_UTILS_DRAW_MAP_REGION_H__

#include <stdint.h>

/**
 * Configuration structure for drawing a tiled map region.
 * Set the fields in draw_map_config before calling draw_map_region().
 */
typedef struct
{
    /** Pointer to the tile index map exported from Tiled (row-major, 1-based indices; 0 = empty). */
    const uint8_t *tiles_map;
    /** Pointer to the raw tile bitmap data (8 bytes per tile, tightly packed). */
    const uint8_t *tiles_data;
    /** Pointer to the SP1 colour-attribute array, one byte per tile (same order as tiles_data). */
    const uint8_t *tiles_attributes;
    /** Width of the map region in tiles. */
    uint8_t map_width;
    /** Height of the map region in tiles. */
    uint8_t map_height;
    /** Screen column (in characters) where the left edge of the map is drawn. */
    uint8_t map_x_offset;
    /** Screen row (in characters) where the top edge of the map is drawn. */
    uint8_t map_y_offset;
    /**
     * Base SP1 tile-slot index used by draw_map_region() to register each tile via sp1_TileEntry.
     * The effective slot for tile N is (sp1_start_tile_entry_index + N).
     * Use distinct ranges for different maps to avoid slot collisions.
     */
    uint8_t sp1_start_tile_entry_index;
} DrawMapRegionConfig;

/**
 * Global configuration used by draw_map_region().
 * Populate this struct before calling draw_map_region().
 */
extern DrawMapRegionConfig draw_map_config;

/**
 * Draws a Tiled-exported tile map region using SP1 tile data and attributes.
 * Each unique tile index in the map gets its own SP1 slot (computed as
 * sp1_start_tile_entry_index + tile_index).
 * The index can be overwritten with another map if it's considered static in a scene
 * and doesn't need to be invalidated (e.g., HUD, fixed/immobile tiles)
 *
 * Note: Set the fields in draw_map_config before calling this function.
 */
void draw_map_region(void);

#endif // __CORE_UTILS_DRAW_MAP_REGION_H__
