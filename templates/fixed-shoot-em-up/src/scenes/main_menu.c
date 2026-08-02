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

void main_menu_scene_init(void) __z88dk_fastcall
{
    zx_border(PAPER_BLACK);
    zx_cls(PAPER_BLACK);

    draw_string_tiles(4u,  MENU_LEFT_COL, MENU_ATTR, MENU_LABEL_0);
    draw_string_tiles(7u,  MENU_LEFT_COL, MENU_ATTR, MENU_LABEL_1);
    draw_string_tiles(9u,  MENU_LEFT_COL, MENU_ATTR, MENU_LABEL_2);
    draw_string_tiles(11u, MENU_LEFT_COL, MENU_ATTR, MENU_LABEL_3);

    draw_string_tiles(18u, 0u,  MENU_ATTR, COPYRIGHT_LABEL_0);
    draw_string_tiles(20u, 0u,  MENU_ATTR, COPYRIGHT_LABEL_1);
    draw_string_tiles(22u, 15u, MENU_ATTR, COPYRIGHT_LABEL_2);
}

void main_menu_scene_update(void) __z88dk_fastcall
{
    if (input_keyboard_pressed(IN_KEY_SCANCODE_0) ||
        input_keyboard_pressed(IN_KEY_SCANCODE_1) ||
        input_keyboard_pressed(IN_KEY_SCANCODE_2) ||
        input_keyboard_pressed(IN_KEY_SCANCODE_3))
    {
        game_state = STATE_PLAYING;
    }
}