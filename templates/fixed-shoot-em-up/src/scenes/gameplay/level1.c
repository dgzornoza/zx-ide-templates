#include "level1.h"
#include "../../z88dk_headers.h"
#include "../features/entities/player.h"
#include "../../core/input/input_manager.h"
#include "../../core/game_definitions.h"
#include "../features/ui/score.h"
#include "../features/ui/hud.h"

void level1_scene_init(void) __z88dk_fastcall
{
    player_init(PLAYER_1);

    hud_init();
}

void level1_scene_update(void) __z88dk_fastcall
{
    /* Frame ordering per scene-entity-architecture:
     *   1. input poll (per-player, zero port I/O after snapshot)
     *   2. player tick (logic + sp1_MoveSprPix dirty-marker)
     *   3. UI tick (score_update conditionally re-prints on score change)
     * Each feature's update handles its own dirty-marker phase internally;,
     * the scene exposes no separate _render callback. The single
     * sp1_UpdateNow() per frame lives in main.c. */

    input_poll(PLAYER_1);

    player_update(PLAYER_1);

    score_update();
}
