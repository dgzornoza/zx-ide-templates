#include <stdint.h>
#include <input.h>
#include <input/input_zx.h>

#include "../../z88dk_headers.h"
#include "../../game_state.h"
#include "../../core/input/input_manager.h"
#include "../../core/utils/sp1_utils.h"

#define START_COL 10u
#define OPTIONS_COUNT 5u

// copyright
#define COPYRIGHT_LABEL_0 "(C) 2026 David Gonzalez"
#define COPYRIGHT_LABEL_1 "fixed-shoot-em-up"
#define COPYRIGHT_LABEL_2 "powered by Zx-Ide"

static const uint8_t menu_option_rows[OPTIONS_COUNT] = {4, 7, 9, 11, 13};
static const char *const menu_option_labels[OPTIONS_COUNT] = {
    "0 Jugar", "1 Keyboard", "2 Kempston", "3 Sinclair", "4 Define keys"};

// state
static uint8_t menu_option_selected = 1u;

static void draw_main_menu(void) __z88dk_fastcall
{
    for (uint8_t i = 0u; i < OPTIONS_COUNT; i++)
    {
        const uint8_t attr = (i == menu_option_selected) ? MENU_ATTR_INVERSE : MENU_ATTR;
        sp1_draw_string(menu_option_rows[i], START_COL, attr, menu_option_labels[i]);
    }

    sp1_draw_string(18, 0, MENU_ATTR, COPYRIGHT_LABEL_0);
    sp1_draw_string(20, 0, MENU_ATTR, COPYRIGHT_LABEL_1);
    sp1_draw_string(22, 15, MENU_ATTR, COPYRIGHT_LABEL_2);
}

static void select_main_menu_option(uint8_t index) __z88dk_fastcall
{
    const uint8_t previous = menu_option_selected;
    menu_option_selected = index;

    if (previous == menu_option_selected)
    {
        return;
    }

    const uint8_t attr_prev = MENU_ATTR;
    sp1_draw_string(menu_option_rows[previous], START_COL, attr_prev, menu_option_labels[previous]);
    const uint8_t attr_cur = MENU_ATTR_INVERSE;
    sp1_draw_string(menu_option_rows[menu_option_selected], START_COL, attr_cur, menu_option_labels[menu_option_selected]);
}

void main_menu_scene_init(void) __z88dk_fastcall
{
    draw_main_menu();
}

void main_menu_scene_update(void) __z88dk_fastcall
{
    // option 0 for play
    // option 4 for define keys
    // options 1,2,3 to select game input

    if (input_keyboard_pressed(IN_KEY_SCANCODE_0))
    {
        game_state = STATE_PLAYING;
    }
    else if (input_keyboard_pressed(IN_KEY_SCANCODE_1))
    {
        select_main_menu_option(1);
    }
    else if (input_keyboard_pressed(IN_KEY_SCANCODE_2))
    {
        select_main_menu_option(2);
    }
    else if (input_keyboard_pressed(IN_KEY_SCANCODE_3))
    {
        select_main_menu_option(3);
    }
    else if (input_keyboard_pressed(IN_KEY_SCANCODE_4))
    {
        game_state = STATE_DEFINE_KEYS;
    }
}
