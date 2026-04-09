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

uint8_t map_y_offset;
uint8_t map_tile_index;

/**
 * Private Map Renderer for SP1.
 * This function uses a single Tile ID (255) as a "volatile" slot for static
 * or mirrored tiles. By forcing an immediate update with sp1_UpdateNow per tile,
 * we can draw an entire mirrored map using only 8 bytes of extra RAM.
 */
static void draw_map(DrawMapRegionParams *params) __z88dk_fastcall
{
    // get all data from the struct, this optimizes the asm code.
    const uint8_t *tiles_map = params->tiles_map;
    uint8_t map_width = params->map_width;
    map_y_offset = params->map_y_offset;
    uint8_t map_x_offset = params->map_x_offset;

    const uint8_t *tiles_data = params->tiles_data;
    const uint8_t *tiles_attr = params->tiles_attributes;

    // loop map y
    for (; map_y_offset < params->map_height; map_y_offset++)
    {
        // for mirrored rows: start at the last tile of this row and read backwards.
        // for normal rows: map_tile_ptr already points to the first tile.
        const uint8_t *map_tile_ptr = has_mirror ? (tiles_map + map_width - 1) : tiles_map;

        // loop map x
        for (uint8_t x = 0; x < map_width; x++)
        {
            // tile index in the map data, which corresponds to the tile bytes to be drawn (and increment pointer for next tile)
            map_tile_index = has_mirror ? *map_tile_ptr-- : *map_tile_ptr++;

            // index 0 in map of tiles is reserved for "empty tile" and should not be drawn
            if (map_tile_index)
            {
                // tile index in Tiled start at 1, so we need to offset them by -1
                // to get the correct tile data and attributes in arrays that are 0-indexed.
                uint8_t tile_index = map_tile_index - 1;
                uint8_t attr = tiles_attr[tile_index];

                const uint8_t *tile_data = tiles_data + (tile_index * 8);
                // calculate current x position with offset and drawing direction
                uint8_t current_map_x_offset = has_mirror ? (map_x_offset - x) : (map_x_offset + x);

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
                    sp1_TileEntry(tile_index, tile_data);
                    sp1_PrintAtInv(map_y_offset, current_map_x_offset, attr, tile_index);
                }

                sp1_UpdateNow();
            }
        }

        // advance sequential pointer to the start of the next row
        tiles_map += map_width;
    }
}

void draw_map_static_region(DrawMapStaticRegionParams *params) __z88dk_fastcall
{
    // force non-mirrored drawing path, which doesn't use the volatile tile slot and is faster.
    use_static_region = 1;
    has_mirror = params->has_mirror;
    draw_map(params);
}

void draw_map_region(DrawMapRegionParams *params) __z88dk_fastcall
{
    use_static_region = 0;
    has_mirror = 0;
    draw_map(params);
}
