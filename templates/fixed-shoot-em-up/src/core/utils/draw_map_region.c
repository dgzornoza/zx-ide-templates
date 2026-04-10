#include "../../z88dk_headers.h"
#include "../definitions.h"
#include "invert_horizontal_tile.h"
#include "draw_map_region.h"

/**
 * Global temporary buffer for tile inversion (8 bytes).
 * This allows us to flip a tile's bitmap in RAM, print it,
 * and reuse the buffer for the next one without storing flipped tilesets.
 */
static uint8_t temp_tile_buffer[8];
static uint8_t use_static_region = 0;
static uint8_t has_mirror = 0;

DrawMapRegionConfig draw_map_config;
#define config draw_map_config

/**
 * Private Map Renderer for SP1.
 * Reads configuration from the global draw_map_config struct, avoiding pointer
 * dereferencing overhead and allowing the compiler to generate tighter ASM code.
 */
static void draw_map(void)
{
    // tiles_map must be local: it is modified each outer iteration (tiles_map += map_width).
    // map_y_offset must be local: it is the outer loop counter (incremented each iteration).
    // All other config fields are accessed directly from draw_map_config:
    //   direct global addressing = ld a,(_sym+n) = 13 T-states
    //   stack-frame local         = ld a,(ix-n)   = 19 T-states  (+6 per access)
    const uint8_t *tiles_map = config.tiles_map;

    for (uint8_t map_y_offset = config.map_y_offset; map_y_offset < config.map_height; map_y_offset++)
    {
        // for mirrored rows: start at the last tile of this row and read backwards.
        // for normal rows: map_tile_ptr already points to the first tile.
        const uint8_t *map_tile_ptr = has_mirror
                                          ? (tiles_map + config.map_width - 1)
                                          : tiles_map;

        // loop map x
        for (uint8_t x = 0; x < config.map_width; x++)
        {
            // tile index in the map data, which corresponds to the tile bytes to be drawn (and increment pointer for next tile)
            uint8_t map_tile_index = has_mirror ? *map_tile_ptr-- : *map_tile_ptr++;

            // index 0 in map of tiles is reserved for "empty tile" and should not be drawn
            if (map_tile_index)
            {
                // tile index in Tiled start at 1, so we need to offset them by -1
                // to get the correct tile data and attributes in arrays that are 0-indexed.
                uint8_t tile_index = map_tile_index - 1;
                uint8_t attr = config.tiles_attributes[tile_index];

                // pointer to the 8 bytes of tile data for this tile index and current x position on screen
                const uint8_t *tile_data = config.tiles_data + (tile_index * 8);
                uint8_t current_map_x_offset = config.map_x_offset + x;

                // mirrored tiles are drawn using a single "volatile" tile slot (255)
                // that gets updated per tile with sp1_UpdateNow to avoid needing a full mirrored tileset in memory.
                if (use_static_region)
                {
                    // invert tile horizontally into the temp buffer
                    if (has_mirror)
                    {
                        TileMirrorParams mirror_params;
                        mirror_params.source = tile_data;
                        mirror_params.dest = temp_tile_buffer;
                        invert_horizontal_tile(&mirror_params);
                    }

                    // update the volatile tile slot (255) with the flipped tile data and print it.
                    sp1_TileEntry(255, has_mirror ? temp_tile_buffer : (uint8_t *)tile_data);
                    sp1_PrintAtInv(map_y_offset, current_map_x_offset, attr, 255);

                    struct sp1_Rect rect;
                    rect.row = map_y_offset;
                    rect.col = current_map_x_offset;
                    rect.width = 1;
                    rect.height = 1;
                    sp1_IterateUpdateRect(&rect, sp1_UpdateNow);
                }
                else
                {
                    uint8_t sp1_slot = config.sp1_start_tile_entry_index + tile_index;
                    sp1_TileEntry(sp1_slot, tile_data);
                    sp1_PrintAtInv(map_y_offset, current_map_x_offset, attr, sp1_slot);
                }

                sp1_UpdateNow();
            }
        }

        // advance sequential pointer to the start of the next row
        tiles_map += config.map_width;
    }
}

void draw_map_static_region(void)
{
    use_static_region = 1;
    has_mirror = 0;
    draw_map();
}

void draw_map_static_region_mirrored(void)
{
    use_static_region = 1;
    has_mirror = 1;
    draw_map();
}

void draw_map_region(void)
{
    use_static_region = 0;
    has_mirror = 0;
    draw_map();
}
