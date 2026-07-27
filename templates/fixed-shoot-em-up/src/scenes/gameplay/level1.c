#include "level1.h"
#include "../features/entities/player.h"
#include "../../core/input/input_manager.h"
#include "../../core/game_definitions.h"
#include "../features/ui/score.h"
#include "../features/ui/hud.h"

void level1_scene_init(void)
{
    player_init(PLAYER_1);

    // Reset all players input state
    input_reset_defaults(PLAYER_1);
#if ALLOWED_GAME_PLAYERS >= 2
    input_reset_defaults(PLAYER_2);
#endif
}

void level1_scene_update(void)
{
    /* Frame ordering per scene-entity-architecture:
     *   1. input snapshot (exactly once per frame)
     *   2. input poll (per-player, zero port I/O after snapshot)
     *   3. player update
     * The single sp1_UpdateNow() per frame stays in main.c. */
    input_keyboard_snapshot();
    input_poll(PLAYER_1);
#if ALLOWED_GAME_PLAYERS >= 2
    input_poll(PLAYER_2);
#endif
    player_update(PLAYER_1);
}

void level1_scene_render(void)
{
    player_render(PLAYER_1);
    score_render();
}
