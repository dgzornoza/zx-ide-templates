#include "score_manager.h"
#include "../z88dk_headers.h"
#include "tile_registry.h"
#include "../data/fonts/numbers.h"
#include "../state/game_state.h"

// Position and format configuration
#define SCORE_X_POS 21
#define SCORE_Y_POS 2
#define SCORE_NUM_DIGITS 6

// Internal memory for tracking changes
static uint16_t last_drawn_score = 0xFFFF; // Impossible by default, forces initial draw

void score_init(void)
{
    // Load the font bitmaps (10 digit tiles) into SP1 graphics memory
    // Mapping digits 0-9 linearly starting from ASCII '0' -> ('0'+0, '0'+1...)
    for (uint8_t i = 0; i < 10; i++)
    {
        sp1_TileEntry(TILE_FONT_NUMBERS_BASE + i, (uint8_t *)numbers + (i * 8));
    }
}

void score_draw(void)
{
    // Check if is playing and the score is different from the last drawn score to avoid unnecessary redraws
    if (game_state == STATE_PLAYING && game_score != last_drawn_score)
    {
        uint16_t temp_score = game_score;

        // Render digit by digit from right to left
        for (int8_t i = SCORE_NUM_DIGITS - 1; i >= 0; i--)
        {
            uint8_t digit = temp_score % 10;
            temp_score /= 10;
            sp1_PrintAtInv(SCORE_Y_POS, SCORE_X_POS + i, INK_WHITE | PAPER_BLACK, TILE_FONT_NUMBERS_BASE + digit);
        }

        last_drawn_score = game_score;
    }
}
