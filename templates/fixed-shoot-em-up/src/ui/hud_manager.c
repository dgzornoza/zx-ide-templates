#include "hud_manager.h"
#include "../z88dk_headers.h"
#include "../data/hud/hud.h"
#include "../data/hud/hud-tiles.h"
#include "../state/game_state.h"
#include "../infrastructure/definitions.h"
#include "../core/utils/invert_horizontal_screen_tile.h"

// hud position
#define HUD_X_OFFSET 0
#define HUD_Y_OFFSET 0

void hud_draw(void)
{
    for (uint8_t y = 0; y < HUD_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < HUD_WIDTH; x++)
        {
            // tile index in hud.asm (is index from hud-tiles.asm)
            uint8_t tile_index = hud[y][x];

            // tile 0 is reserved for empty tile, so we only draw if tile_index > 0
            if (tile_index > 0)
            {
                // offset used in hud-tiles.asm (tile_index - 1 because tile_index starts from 1, but array index starts from 0)
                uint8_t offset = tile_index - 1U;
                uint8_t tile_attribute = hud_tiles_attributes[offset];

                // register tiles
                const uint8_t *hud_tile = hud_tiles + (offset * CHAR_SIZE);
                sp1_TileEntry(tile_index, hud_tile);

                // draw original tile
                sp1_PrintAtInv(HUD_Y_OFFSET + y, HUD_X_OFFSET + x, tile_attribute, tile_index);
                // draw copy tile in right screen for mirroring tiles (inverted from left screen),
                // Note: use SCREEN_CHARS_WIDTH_BASE_0 because is from 0 to 31 (32 chars per row)
                uint8_t mirrored_x = SCREEN_CHARS_WIDTH_BASE_0 - HUD_X_OFFSET - x;
                sp1_PrintAtInv(HUD_Y_OFFSET + y, mirrored_x, tile_attribute, tile_index);

                sp1_UpdateNow();
                void *address = zx_cxy2saddr(mirrored_x, HUD_Y_OFFSET + y);
                invert_horizontal_screen_char(address);
            }
        }
    }
}
