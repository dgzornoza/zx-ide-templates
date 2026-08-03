#include <stdint.h>
#include <input.h>
#include <input/input_zx.h>

#include "../z88dk_headers.h"
#include "../game_state.h"
#include "../core/input/input_manager.h"
#include "../core/utils/draw_string_tiles.h"

#define MENU_LEFT_COL 10u

#define MENU_LABEL_0 "0 Jugar"
#define MENU_LABEL_1 "1 Keyboard"
#define MENU_LABEL_2 "2 Kempston"
#define MENU_LABEL_3 "3 Sinclair"

#define COPYRIGHT_LABEL_0 "(C) 2026 David Gonzalez"
#define COPYRIGHT_LABEL_1 "fixed-shoot-em-up"
#define COPYRIGHT_LABEL_2 "powered by Zx-Ide"

#define MENU_ATTR (INK_WHITE | PAPER_BLACK)
#define MENU_ATTR_INVERSE (INK_BLACK | PAPER_WHITE)

static const uint8_t menu_option_rows[4] = {4, 7, 9, 11};
static const char *const menu_option_labels[4] = {
    MENU_LABEL_0, MENU_LABEL_1, MENU_LABEL_2, MENU_LABEL_3};

static uint8_t menu_selected = 1;

static void draw_menu_option(uint8_t index) __z88dk_fastcall
{
    const uint8_t attr = (index == menu_selected) ? MENU_ATTR_INVERSE : MENU_ATTR;
    draw_string_tiles(menu_option_rows[index], MENU_LEFT_COL, attr, menu_option_labels[index]);
}

static void select_menu_option(uint8_t index) __z88dk_fastcall
{
    const uint8_t previous = menu_selected;
    menu_selected = index;

    if (previous != menu_selected)
    {
        draw_menu_option(previous);
        draw_menu_option(menu_selected);
    }
}

void main_menu_scene_init(void) __z88dk_fastcall
{
    zx_border(PAPER_BLACK);
    zx_cls(PAPER_BLACK);

    for (uint8_t i = 0; i < 4; i++)
    {
        draw_menu_option(i);
    }

    // copyright
    draw_string_tiles(18, 0, MENU_ATTR, COPYRIGHT_LABEL_0);
    draw_string_tiles(20, 0, MENU_ATTR, COPYRIGHT_LABEL_1);
    draw_string_tiles(22, 15, MENU_ATTR, COPYRIGHT_LABEL_2);
}

void main_menu_scene_update(void) __z88dk_fastcall
{
    if (input_keyboard_pressed(IN_KEY_SCANCODE_0))
    {
        game_state = STATE_PLAYING;
    }
    else if (input_keyboard_pressed(IN_KEY_SCANCODE_1))
    {
        select_menu_option(1);
    }
    else if (input_keyboard_pressed(IN_KEY_SCANCODE_2))
    {
        select_menu_option(2);
    }
    else if (input_keyboard_pressed(IN_KEY_SCANCODE_3))
    {
        select_menu_option(3);
    }
}
