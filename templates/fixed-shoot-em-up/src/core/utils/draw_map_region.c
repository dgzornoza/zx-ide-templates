#include "../../z88dk_headers.h"
#include "../system_definitions.h"
#include "../infrastructure/graphics.h"
#include "draw_map_region.h"

DrawMapRegionConfig draw_map_config;
#define config draw_map_config

void draw_map_region(void)
{
    const uint8_t *tiles_map = config.tiles_map;

    // loop map y
    for (uint8_t y = 0; y < config.map_height; y++)
    {
        // map_tile_ptr walks the current row left-to-right.
        const uint8_t *map_tile_ptr = tiles_map;
        uint8_t current_map_y_offset = config.map_y_offset + y;

        // loop map x
        for (uint8_t x = 0; x < config.map_width; x++)
        {
            // tile index in the map data, advance pointer for next tile.
            uint8_t map_tile_index = *map_tile_ptr++;

            // index 0 in map of tiles is reserved for "empty tile" and should not be drawn.
            if (map_tile_index)
            {
                // tile index in Tiled starts at 1, so we offset by -1 to get the
                // correct tile in arrays that are 0-indexed. The slot is
                // base + tile_index, graphics and attributes live at that slot
                // in the shared tile buffer (graphics.h).
                uint8_t tile_index = map_tile_index - 1;
                uint8_t tile_slot = config.start_tile_entry_index + tile_index;
                uint8_t attr = tile_attr[tile_slot];
                uint8_t current_map_x_offset = config.map_x_offset + x;

                // invalidate and print tile in next update.
                sp1_PrintAtInv(current_map_y_offset, current_map_x_offset, attr, tile_slot);
            }
        }

        // advance sequential pointer to the start of the next row
        tiles_map += config.map_width;
    }
}
