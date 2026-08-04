#include "./z88dk_headers.h"
#include "core/infrastructure/isr.h"
#include "core/input/input_manager.h"
#include "scenes/splash.h"
#include "scenes/main_menu.h"
#include "scenes/define_keys_menu.h"
#include "scenes/gameplay/level1.h"
#include "game_state.h"
#include "scenes/features/ui/hud.h"
#include "scenes/features/ui/score.h"
#include <intrinsic.h>
#include <string.h>

const struct sp1_Rect game_area = {0, 0, 32, 24};

/* One-shot flags so _init() function runs only on first entry. */
static uint8_t level1_inited = 0u;
static uint8_t main_menu_inited = 0u;
static uint8_t define_keys_menu_inited = 0u;

/** Main app entry point */
int main(void)
{
    im2_setup();

    /* Initialize SP1 with rotation tables ONLY. OVERWRITE_DFILE and
     * OVERWRITE_TILES are intentionally ABSENT so the load-screen.scr
     * pixels loaded by the .tap bootstrap survive SP1 init. The splash
     * overlays its prompt on row 20 via sp1_TileEntry/sp1_PrintAt and
     * the rest of the loaded art remains visible. */
    // sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL, INK_WHITE | PAPER_BLACK, ' ');
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, INK_WHITE | PAPER_BLACK, ' ');

    /* One-shot splash setup.
     * No sp1_Invalidate here - would mark cells dirty without attached
     * structs and clobber the loaded art. */
    splash_scene_init();

    /* main app loop */
    while (1)
    {
        /* Wait for the next frame tick (50 ms) */
        uint8_t current_tick = frame_tick;
        while (current_tick == frame_tick)
        {
            intrinsic_halt();
        }

        /* Dispatch by current game state. */
        switch (game_state)
        {
        case STATE_SPLASH:
            splash_scene_update();
            break;
        case STATE_MENU:
            /* Coming back from define keys: drop both _inited flags so the
             * main menu redraws on entry and the next define-keys visit
             * starts from a clean state. */
            if (define_keys_menu_inited)
            {
                define_keys_menu_inited = 0u;
                main_menu_inited = 0u;
            }
            if (!main_menu_inited)
            {
                main_menu_scene_init();
                main_menu_inited = 1u;
            }
            main_menu_scene_update();
            break;

        case STATE_DEFINE_KEYS:
            if (!define_keys_menu_inited)
            {
                define_keys_menu_scene_init();
                define_keys_menu_inited = 1u;
            }
            define_keys_menu_scene_update();
            break;

        case STATE_PLAYING:
            if (!level1_inited)
            {
                level1_scene_init();
                level1_inited = 1u;
            }
            level1_scene_update();
            break;

        case STATE_GAME_OVER:
            /* Halt placeholder until the game over scene lands. */
            for (;;)
            {
                intrinsic_halt();
            }
            /* not reached */
            break;
        default:
            /* Unknown state: halt so we never spin into undefined behaviour. */
            for (;;)
            {
                intrinsic_halt();
            }
            /* not reached */
            break;
        }

        /* Single sp1_UpdateNow() site: flashes the splash prompt via ULA
         * FLASH, drives every other SP1-managed cell. */
        sp1_UpdateNow();
    }
}
