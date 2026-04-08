#include "draw_tiled_data.h"
#include "../../z88dk_headers.h"
#include "../definitions.h"
#include "invert_horizontal_screen_tile.h"

// alias for easier read draw_tiled_config variable.
#define configuration draw_tiled_config
DrawTiledConfig draw_tiled_config;

void draw_tiled_data(void) __z88dk_fastcall
{
    const uint8_t *tile_map_pointer = configuration.tile_map;

    // invalidate map area for redraw
    const struct sp1_Rect map_area = {configuration.map_x_offset, configuration.map_y_offset, configuration.map_width, configuration.map_height};
    sp1_Invalidate(&map_area);

    // Iterate through the tile map and draw each tile using SP1
    for (uint8_t y = 0; y < configuration.map_height; y++)
    {
        for (uint8_t x = 0; x < configuration.map_width; x++)
        {
            uint8_t tile_index = *tile_map_pointer;
            tile_map_pointer++;

            // Tiled uses tile_index 0 as empty cell in the exported map.
            if (tile_index > 0)
            {
                uint8_t offset = tile_index - 1U;
                uint8_t tile_attribute = configuration.tiles_attributes[offset];
                const uint8_t *tile_bitmap = configuration.tiles_data + ((uint16_t)offset * CHAR_SIZE);

                // draw tile (this not show in screen until sp1_UpdateNow is called)
                sp1_TileEntry(tile_index, tile_bitmap);
                sp1_PrintAtInv(configuration.map_y_offset + y, configuration.map_x_offset + x, tile_attribute, tile_index);

                // If mirroring is enabled, draw the mirrored copy with inverted graphics.
                // is less efficient because it forces the tile to be drawn, then inverts the tile shown on the screen.
                // But it's a good way to save memory by not having to generate inverted tiles in the data.
                // since the tiles are static, the performance impact is minimal.
                if (configuration.has_mirror)
                {
                    // draw copy tile in right screen for mirroring tiles (inverted from left screen),
                    // Note: use SCREEN_CHARS_WIDTH_BASE_0 because is from 0 to 31 (32 chars per row)
                    uint8_t mirrored_x = SCREEN_CHARS_WIDTH_BASE_0 - configuration.map_x_offset - x;
                    sp1_PrintAtInv(configuration.map_y_offset + y, mirrored_x, tile_attribute, tile_index);

                    // show in screen only the affected tile mirrored
                    struct sp1_Rect update_rect = {configuration.map_y_offset + y, mirrored_x, 1, 1};
                    sp1_IterateUpdateRect(&update_rect, sp1_UpdateNow);

                    void *address = zx_cxy2saddr(mirrored_x, configuration.map_y_offset + y);
                    invert_horizontal_screen_char(address);
                }
            }
        }
    }
}
