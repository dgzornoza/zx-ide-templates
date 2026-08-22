#include "./z88dk_headers.h"
#include "core/infrastructure/isr.h"
#include "core/infrastructure/timer.h"
#include "scenes/scene_dispatcher.h"
#include "core/input/input_manager.h"
#include "core/game_definitions.h"
#include <intrinsic.h>

int main(void)
{
    im2_setup();

    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
                   DEFAULT_SCREEN_INK_COLOR | DEFAULT_SCREEN_PAPER_COLOR, ' ');

    /** Reset inputs to defaults */
    input_reset_defaults(PLAYER_1);

    /** Initialize scene dispatcher */
    scene_dispatcher_init();

    while (1)
    {
        uint16_t current_tick = GLOBAL_TIMER_ABS_TICKS;
        while (current_tick == GLOBAL_TIMER_ABS_TICKS)
        {
            intrinsic_halt();
        }

        /** update scene dispatcher */
        scene_dispatcher_update();

        /** update sp1 graphics */
        sp1_UpdateNow();

        // Alternativa a probar ----------------------------------
        // uint16_t last_tick = GLOBAL_TIMER_ABS_TICKS;

        // // Lógica SIEMPRE
        // scene_dispatcher_update();

        // // Render SOLO cuando toca
        // if (GLOBAL_TIMER_ABS_TICKS != last_tick)
        // {
        //     sp1_UpdateNow();
        //     last_tick = GLOBAL_TIMER_ABS_TICKS;
        // }
    }
}
