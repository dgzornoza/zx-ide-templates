#include "./z88dk_headers.h"
#include "core/infrastructure/isr.h"
#include "scenes/gameplay/level1.h"
#include "game_state.h"
#include "scenes/features/ui/hud.h"
#include "scenes/features/ui/score.h"
#include <intrinsic.h>
#include <string.h>

const struct sp1_Rect game_area = {0, 0, 32, 24};

/** Main app entry point */
int main(void)
{
    im2_setup();

    // limpiar pantalla
    zx_border(PAPER_BLACK);
    zx_cls(PAPER_BLACK);

    // Initialize SP1 engine with rotation tables and clear settings
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, INK_WHITE | PAPER_BLACK, ' ');

    // Mark entire screen as "dirty" for the initial full render
    sp1_Invalidate(&game_area);

    // Initialize global state
    game_state_init();

    // Initialize main menu
    // TODO: temporal hasta que este el menu principal
    level1_scene_init();

    // main app loop
    while (1)
    {
        // Wait for the next frame tick (50 ms)
        uint8_t current_tick = frame_tick;
        while (current_tick == frame_tick)
        {
            intrinsic_halt();
        }

        // update game state (movement, collisions, etc.)
        level1_scene_update();
        level1_scene_render();

        // update sp1 scene
        sp1_UpdateNow();
    }
}
