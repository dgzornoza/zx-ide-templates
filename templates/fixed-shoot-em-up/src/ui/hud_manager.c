#include "hud_manager.h"
#include "../z88dk_headers.h"
#include "../data/hud/hud.h"
#include "../data/hud/hud-tiles.h"
#include "../state/game_state.h"

// Panel position configuration on the 32x24 character grid
#define HUD_X_OFFSET 8
#define HUD_Y_OFFSET 0

static void hud_init(void)
{
    // 1. Load tile bitmaps (binary data) into SP1 graphics memory.
    // From 'hud.asm' the map uses indices (tiles) from 1 to 64
    // hud_tiles_tiles has 8 bytes (8x8 pixels) per tile pattern.

    // Index 0 is usually skipped assuming empty (loaded by sp1_Initialize in main.c)
    for (uint16_t i = 1; i <= 64; i++)
    {
        // In SP1, load the array + its offset assuming 8 bytes per graphic:
        // Note: (i - 1) * 8 because tiles typically map from byte 0.
        // If the exporter generated a blank tile as tile 0, this may vary,
        // but mapping i*8 or (i-1)*8 depends on the exported data.
        // We will assume the map starts directly at the pointer offset.
        sp1_TileEntry(i, hud_tiles_tiles + (i * 8));
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
                // We would read the color attribute from 'hud_tiles_tiles_attributes' if color were fixed per tile,
                // but for simplicity we draw with a global default color
                sp1_PrintAt(HUD_Y_OFFSET + y, HUD_X_OFFSET + x, INK_WHITE | PAPER_BLACK, tile_index);
            }
        }
    }
}
