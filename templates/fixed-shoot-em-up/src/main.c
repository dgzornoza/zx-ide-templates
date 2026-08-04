#include "./z88dk_headers.h"
#include "core/infrastructure/isr.h"
#include "scenes/scene_dispatcher.h"
#include <intrinsic.h>

const struct sp1_Rect game_area = {0, 0, 32, 24};

int main(void)
{
    im2_setup();

    sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE, INK_WHITE | PAPER_BLACK, ' ');
    scene_dispatcher_init();

    while (1)
    {
        uint8_t current_tick = frame_tick;
        while (current_tick == frame_tick)
        {
            intrinsic_halt();
        }

        scene_dispatcher_update();
        sp1_UpdateNow();
    }
}
