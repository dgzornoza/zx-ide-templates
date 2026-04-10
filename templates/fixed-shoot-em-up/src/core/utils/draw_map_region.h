#ifndef __CORE_UTILS_DRAW_MAP_REGION_H__
#define __CORE_UTILS_DRAW_MAP_REGION_H__

#include <stdint.h>

/**
 * Configuration structure for drawing a tiled map region.
 * Set the fields in draw_map_config before calling any draw_map_* function.
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
     * NOTE: only used by draw_map_region(). draw_map_static_region() and
     * draw_map_static_region_mirrored() always use the volatile slot 255 and ignore this field.
     */
    uint8_t sp1_start_tile_entry_index;
} DrawMapRegionConfig;

/**
 * Global configuration used by all draw_map_* functions.
 * Populate this struct before calling any draw_map_* function.
 */
extern DrawMapRegionConfig draw_map_config;

/**
 * Draws a static map region using SP1 tile data and attributes.
 * Uses a single Tile ID (255) as a "volatile" slot for immediate rendering.
 * Note: If the tile is redrawn or a sprite passes over it, the tile will contain garbage.
 * Use ONLY for regions that do not change and where nothing will overlap.
 *
 * Note: Set the fields in draw_map_config before calling this function
 */
extern void draw_map_static_region(void);

/**
 * Draws a horizontally mirrored static map region using SP1 tile data and attributes.
 * Uses a single Tile ID (255) as a "volatile" slot, inverting each tile's graphics.
 * Note: If the tile is redrawn or a sprite passes over it, the tile will contain garbage.
 * Use ONLY for regions that do not change and where nothing will overlap.
 *
 * @remarks Set the fields in draw_map_config before calling any draw_map_* function.
 */
extern void draw_map_static_region_mirrored(void);

/**
 * Draws a Tiled-exported tile map region using SP1 tile data and attributes.
 * Tiles are stored at their tile_index slot. Calling again with the same indices overwrites them.
 * Manage game map tile indices carefully to avoid unintended overwrites.
 *
 * Note: Set the fields in draw_map_config before calling this function
 */
extern void draw_map_region(void);

#endif // __CORE_UTILS_DRAW_MAP_REGION_H__
