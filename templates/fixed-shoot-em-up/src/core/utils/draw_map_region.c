#include "../../z88dk_headers.h"
#include "../system_definitions.h"
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
            // tile index in the map data; advance pointer for next tile.
            uint8_t map_tile_index = *map_tile_ptr++;

            // index 0 in map of tiles is reserved for "empty tile" and should not be drawn.
            if (map_tile_index)
            {
                // tile index in Tiled starts at 1, so we offset by -1 to get the
                // correct tile data and attributes in arrays that are 0-indexed.
                uint8_t tile_index = map_tile_index - 1;
                uint8_t attr = config.tiles_attributes[tile_index];

                // pointer to the 8 bytes of tile data for this tile index,
                // and the screen column where this cell will be drawn.
                const uint8_t *tile_data = config.tiles_data + (tile_index * 8);
                uint8_t current_map_x_offset = config.map_x_offset + x;

                // Each unique tile gets its own SP1 slot so the graphic is
                // captured at PrintAt time and never overwritten by neighbours.
                uint8_t sp1_slot = config.sp1_start_tile_entry_index + tile_index;
                sp1_TileEntry(sp1_slot, tile_data);
                sp1_PrintAtInv(current_map_y_offset, current_map_x_offset, attr, sp1_slot);

                struct sp1_Rect rect;
                rect.row = current_map_y_offset;
                rect.col = current_map_x_offset;
                rect.width = 1;
                rect.height = 1;
                sp1_IterateUpdateRect(&rect, sp1_UpdateNow);
                // sp1_UpdateNow();
            }
        }

        // advance sequential pointer to the start of the next row
        tiles_map += config.map_width;
    }
}
