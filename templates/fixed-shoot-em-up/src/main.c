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
                   SCREEN_COLOR, ' ');

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

        /** upadte scene dispatcher */
        scene_dispatcher_update();

        /** update sp1 graphics */
        sp1_UpdateNow();
    }
}
