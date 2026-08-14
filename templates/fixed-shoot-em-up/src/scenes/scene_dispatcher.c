#include <intrinsic.h>

#include "scene_dispatcher.h"
#include "../game_state.h"
#include "system/splash.h"
#include "system/main_menu.h"
#include "system/define_keys_menu.h"
#include "gameplay/level1.h"
#include "../core/utils/sp1_utils.h"

static GameState active_game_state;

static void halt(void) __z88dk_fastcall
{
    for (;;)
    {
        intrinsic_halt();
    }
}

static void init_active_scene(void) __z88dk_fastcall
{
    switch (active_game_state)
    {
    case STATE_SPLASH:
        splash_scene_init();
        break;
    case STATE_MENU:
        main_menu_scene_init();
        break;
    case STATE_DEFINE_KEYS:
        define_keys_menu_scene_init();
        break;
    case STATE_PLAYING:
        level1_scene_init();
        break;
    case STATE_GAME_OVER:
    default:
        halt();
        break;
    }
}

void scene_dispatcher_init(void) __z88dk_fastcall
{
    active_game_state = game_state;
    init_active_scene();
}

void scene_dispatcher_update(void) __z88dk_fastcall
{
    if (game_state != active_game_state)
    {
        active_game_state = game_state;

        /* Retire the previous scene's tile references before the new scene draws anything */
        sp1_clear_screen_tiles();

        init_active_scene();
    }

    switch (active_game_state)
    {
    case STATE_SPLASH:
        splash_scene_update();
        break;
    case STATE_MENU:
        main_menu_scene_update();
        break;
    case STATE_DEFINE_KEYS:
        define_keys_menu_scene_update();
        break;
    case STATE_PLAYING:
        level1_scene_update();
        break;
    case STATE_GAME_OVER:
    default:
        halt();
        break;
    }
}
