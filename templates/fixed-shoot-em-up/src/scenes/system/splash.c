/*
 * splash.c - First-boot splash for ZX-IDE fixed-shoot-em-up template.
 *
 * Runs AFTER sp1_Initialize (which deliberately omits OVERWRITE_DFILE so the
 * loaded load-screen.scr pixels survive) and BEFORE the central dispatch loop.
 * Registers font_1 ASCII tiles via sp1_TileEntry and prints a "PULSA UNA
 * TECLA" prompt on row 20 with the FLASH attribute bit, then polls in_inkey()
 * (lowercase - sdcc_iy clib symbol from <input/input_zx.h>) per frame and
 * transitions game_state to STATE_MENU on a rising-edge keypress.
 */

#include <stdint.h>
#include <input.h>
#include <input/input_zx.h>
#include <intrinsic.h>

#include "../../z88dk_headers.h"
#include "../features/ui/tile_registry.h"
#include "../../data/fonts/font_1.h"
#include "../../game_state.h"
#include "../../core/utils/sp1_utils.h"

/* "PULSA UNA TECLA" = 15 chars, centred on the 32-column Spectrum display. */
#define PROMPT_TEXT "PULSA UNA TECLA"
#define PROMPT_LEN 15u
#define PROMPT_ROW 20u
#define PROMPT_COL ((32u - PROMPT_LEN) / 2u) /* = 8 */

/* Rising-edge debounce sentinel: 0xFFu prevents a key held at boot from
 * satisfying the very first poll because cur is never 0xFFu (only 0 or 1). */
static uint8_t prev_key = 0xFFu;

void splash_scene_init(void) __z88dk_fastcall
{

    /* Register the font_1 ASCII range. Tile IDs are ASCII-aligned per
     * TILE_FONT_BASE = ' '; each glyph is 8 bytes at font_1 + (c - 32) * 8. */
    for (uint8_t i = 0; i < TILE_FONT_LEN; i++)
    {
        sp1_TileEntry(TILE_FONT_START + i, (uint8_t *)font_1 + (i * 8u));
    }

    /* Print the prompt on row 20 with the FLASH attribute bit so the ULA
     * toggles INK/PAPER automatically at ~2 Hz - zero per-frame CPU work. */
    sp1_draw_string(PROMPT_ROW, PROMPT_COL, INK_WHITE | PAPER_BLACK | FLASH, PROMPT_TEXT);
}

void splash_scene_update(void) __z88dk_fastcall
{
    /* wait to keypress detection to change to menu state.
     * in_inkey() returns 0 when no key is
     * pressed and non-zero when any key is pressed, normalize to 0/1.
     */
    uint8_t cur = in_inkey() ? 1u : 0u;
    if (prev_key == 0u && cur != 0u)
    {
        game_state = STATE_MENU;
    }
    prev_key = cur;
}
