#include "level1.h"
#include "../features/entities/player.h"
#include "../../core/input/input_manager.h"
#include "../features/ui/score.h"

void level1_scene_init(void)
{
    player_init();
}

void level1_scene_update(void)
{
    input_poll();
    player_update();
}

void level1_scene_render(void)
{
    player_render();
    score_render();
}
