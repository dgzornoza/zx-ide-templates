#include "hud_manager.h"
#include "../z88dk_headers.h"
#include "../data/hud/hud.h"
#include "../data/hud/hud-tiles.h"
#include "../state/game_state.h"

// Panel position configuration on the 32x24 character grid
#define HUD_X_OFFSET 0
#define HUD_Y_OFFSET 0

static void hud_init(void)
{
    // 1. Load tile bitmaps (binary data) into SP1 graphics memory.
    // From 'hud.asm' the map uses indices (tiles) from 1 to 64
    // hud_tiles_tiles has 8 bytes (8x8 pixels) per tile pattern.

    // Note: Since the HUD is static and loaded first, it doesn’t need to update, the tiles can be loaded at any index,
    // and then those indices can be used by any other part of the program.
    // It isn’t necessary to reserve a dedicated block of indices exclusively for the HUD.

    // Index 0 is usually skipped assuming empty
    for (uint16_t i = 1; i <= 64; i++)
    {
        // Tile index starts at 1 in the map, while tile data starts at byte 0.
        sp1_TileEntry(i, hud_tiles + ((i - 1U) * 8U));
    }
}

void hud_draw(void)
{
    // Initialize the HUD graphics in SP1 first
    hud_init();

    // 2. Draw the static panel grid that does not change (16 columns x 4 rows)
    // Reading indices directly from our exported hud[4][16] array

    for (uint8_t y = 0; y < HUD_HEIGHT; y++)
    {
        for (uint8_t x = 0; x < HUD_WIDTH; x++)
        {
            uint8_t tile_index = hud[y][x];

            // Assign tile 0 (empty) for zeros, or the default color (e.g., INK_WHITE)
            if (tile_index > 0)
            {
                uint8_t tile_attribute = hud_tiles_attributes[tile_index - 1U];
                sp1_PrintAt(HUD_Y_OFFSET + y, HUD_X_OFFSET + x, tile_attribute, tile_index);
            }
        }
    }
}
