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

/**
 * Map Renderer for SP1.
 * This function uses a single Tile ID (255) as a "volatile" slot for static
 * or mirrored tiles. By forcing an immediate update with sp1_UpdateNow per tile,
 * we can draw an entire mirrored map using only 8 bytes of extra RAM.
 */
void draw_map_region(DrawTiledParams *params) __z88dk_fastcall
{
    // get all data from the struct, this optimizes the asm code.
    const uint8_t *tile_map = params->tile_map;
    uint8_t map_width = params->map_width;
    uint8_t map_y = params->map_y_offset;
    uint8_t map_x = params->map_x_offset;
    uint8_t has_mirror = params->has_mirror;

    const uint8_t *tiles_data = params->tiles_data;
    const uint8_t *tiles_attr = params->tiles_attributes;

    // loop map y
    for (uint8_t y = 0; y < params->map_height; y++, map_y++)
    {
        const uint8_t *row = has_mirror ? (tile_map + map_width - 1) : tile_map;

        // loop map x
        for (uint8_t x = 0; x < map_width; x++)
        {
            uint8_t tile_index = has_mirror ? *row-- : *row++;

            // index 0 is reserved for "empty tile" and should not be drawn
            if (tile_index)
            {
                // tile indices in Tiled start at 1, so we need to offset them by -1
                // to get the correct tile data and attributes in arrays that are 0-indexed.
                uint8_t offset = tile_index - 1;
                uint8_t attr = tiles_attr[offset];

                const uint8_t *tile = tiles_data + (offset * 8);
                uint8_t map_x_offset = has_mirror ? (map_x - x) : (map_x + x);

                // mirrored tiles are drawn using a single "volatile" tile slot (255)
                // that gets updated per tile with sp1_UpdateNow to avoid needing a full mirrored tileset in memory.
                if (has_mirror)
                {
                    TileMirrorParams m_args;
                    m_args.source = tile;
                    m_args.dest = temp_tile_buffer;
                    invert_horizontal_tile(&m_args);

                    sp1_TileEntry(255, temp_tile_buffer);
                    sp1_PrintAtInv(map_y, map_x_offset, attr, 255);

                    struct sp1_Rect rect;
                    rect.row = map_y;
                    rect.col = map_x_offset;
                    rect.width = 1;
                    rect.height = 1;
                    sp1_IterateUpdateRect(&rect, sp1_UpdateNow);
                }
                else
                {
                    sp1_TileEntry(tile_index, tile);
                    sp1_PrintAtInv(map_y, map_x_offset, attr, tile_index);
                }
            }
        }

        tile_map += map_width;
    }
}
