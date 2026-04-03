#include "./z88dk_headers.h"
#include "infrastructure/isr.h"
#include "state/game_state.h"
#include "ui/hud_manager.h"
#include "ui/score_manager.h"
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
    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, INK_BLACK | PAPER_BLACK, ' ');

    // Mark entire screen as "dirty" for the initial full render
    sp1_Invalidate(&game_area);

    // Initialize global state
    game_state_init();

    // Initialize visual components
    hud_draw();
    // score_init();
    // score_draw();

    // main app loop
    while (1)
    {
        // Wait for the next frame tick (50 ms)
        uint8_t current_tick = frame_tick;
        while (current_tick == frame_tick)
        {
            intrinsic_halt();
        }

        score_draw();

        // actualizar sp1
        sp1_UpdateNow();
    }
}
