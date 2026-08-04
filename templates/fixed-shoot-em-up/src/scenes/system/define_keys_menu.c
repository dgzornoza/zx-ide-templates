#include <stdint.h>
#include <input.h>
#include <input/input_zx.h>

#include "../../z88dk_headers.h"
#include "../../game_state.h"
#include "../../core/input/input_manager.h"
#include "../../core/utils/draw_string_tiles.h"

#define DEFINE_KEYS_START_COL 10u
#define DEFINE_KEYS_BOUND_CHAR_COL 20u

enum
{
    define_keys_options_count = 3u
};

static const uint8_t define_keys_menu_option_rows[define_keys_options_count] = {5, 7, 9};
static const char *const define_keys_menu_option_labels[define_keys_options_count] = {"IZQUIERDA:", "DERECHA:", "DISPARO:"};

// state
static InputBindings define_keys_bindings;
// ASCII char rendered to the right of each label. 0 = unassigned.
static char define_keys_bound_chars[define_keys_options_count] = {0};
// char + '\0' buffer draw_string_tiles requires (no NUL-terminated inline form).
static char define_keys_bound_char_text[2];
static uint8_t menu_option_selected = 0u;
static uint8_t define_keys_wait_release = 0u;

static void draw_define_keys_row(uint8_t index) __z88dk_fastcall
{
    const uint8_t attr = (index == menu_option_selected) ? MENU_ATTR_INVERSE : MENU_ATTR;
    draw_string_tiles(define_keys_menu_option_rows[index], DEFINE_KEYS_START_COL, attr, define_keys_menu_option_labels[index]);

    const char ch = define_keys_bound_chars[index];
    define_keys_bound_char_text[0] = (ch == ' ' || ch == 0) ? '_' : ch;
    define_keys_bound_char_text[1] = '\0';
    draw_string_tiles(define_keys_menu_option_rows[index], DEFINE_KEYS_BOUND_CHAR_COL, attr, define_keys_bound_char_text);
}

/** Draw define keys menu */
static void draw_define_keys_menu(void) __z88dk_fastcall
{
    zx_cls(PAPER_BLACK);

    for (uint8_t i = 0u; i < define_keys_options_count; i++)
    {
        draw_define_keys_row(i);
    }
}

/** Function for set option for define key */
static void select_define_keys_option(uint8_t index) __z88dk_fastcall
{
    const uint8_t previous = menu_option_selected;
    menu_option_selected = index;

    if (previous == menu_option_selected)
    {
        return;
    }

    draw_define_keys_row(previous);
    draw_define_keys_row(menu_option_selected);
}

void define_keys_menu_scene_init(void) __z88dk_fastcall
{
    define_keys_bindings.left = 0u;
    define_keys_bindings.right = 0u;
    define_keys_bindings.up = 0u;
    define_keys_bindings.down = 0u;
    define_keys_bindings.fire1 = 0u;
    menu_option_selected = 0u;
    define_keys_wait_release = (in_inkey() == 0) ? 0u : 1u;
    draw_define_keys_menu();
}

void define_keys_menu_scene_update(void) __z88dk_fastcall
{
    const int ch = in_inkey();

    // wait to release key
    if (ch == 0)
    {
        if (define_keys_wait_release != 0u)
        {
            define_keys_wait_release = 0u;
        }
        return;
    }
    if (define_keys_wait_release != 0u)
    {
        return;
    }

    const uint16_t scan = (uint16_t)in_key_scancode((unsigned char)ch);
    const uint8_t current = menu_option_selected;

    if (current == 0u)
    {
        define_keys_bindings.left = scan;
    }
    else if (current == 1u)
    {
        define_keys_bindings.right = scan;
    }
    else
    {
        define_keys_bindings.fire1 = scan;
    }
    define_keys_bound_chars[current] = (char)ch;
    define_keys_wait_release = 1u;

    // last slot: commit and bounce back to the main menu without advancing.
    if (current + 1u >= define_keys_options_count)
    {
        draw_define_keys_row(current);
        input_set_keyboard_bindings(PLAYER_1, &define_keys_bindings);
        game_state = STATE_MENU;
        return;
    }
    select_define_keys_option(current + 1u);
}
