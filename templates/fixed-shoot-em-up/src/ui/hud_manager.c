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

/**
 *  Private function to invert the right screen HUD tiles after drawing the original tiles in the left screen
 */
static void invert_right_screen_hud(void);

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
                sp1_PrintAt(HUD_Y_OFFSET + y, HUD_X_OFFSET + x, tile_attribute, tile_index);
                // draw copy tile in right screen for mirroring tiles, not set attribute to avoid show before of mirroring tile
                sp1_PrintAt(HUD_Y_OFFSET + y, HUD_X_OFFSET + HUD_WIDTH + x, 1, tile_index);
            }
        }
    }

    // update SP1 for draw all tiles before mirroring tiles in right screen
    sp1_UpdateNow();

    invert_right_screen_hud();
}

uint8_t *address;
uint8_t y;
uint8_t x;
static void invert_right_screen_hud(void)
{
    // mirror tiles in right screen
    for (y = 0; y < HUD_HEIGHT; y++)
    {
        for (x = HUD_WIDTH; x < SCREEN_CHARS_WIDTH; x++)
        {
            address = zx_cxy2aaddr(x, y);

            // invert_horizontal_screen_tile(address);

            *address = 0xFF; // fila completa de píxeles encendidos
            // for (uint8_t i = 0; i < 8; i++)
            // {
            //     address[i] = 0xFF; // fila completa de píxeles encendidos
            // }
        }
    }
}
