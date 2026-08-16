#include <stdint.h>
#include <input.h>

#include "../../z88dk_headers.h"
#include "../../game_state.h"
#include "../../core/input/input_manager.h"
#include "../../core/game_definitions.h"
#include "../../core/utils/sp1_utils.h"

#define START_COL 10u
#define BOUND_CHAR_COL 22u
#define OPTIONS_COUNT 3u

static const uint8_t menu_option_rows[OPTIONS_COUNT] = {5u, 7u, 9u};
static const char *const menu_option_labels[OPTIONS_COUNT] = {"IZQUIERDA:", "DERECHA:", "DISPARO:"};
static uint8_t menu_option_selected = 0u;

// binding keys
static InputBindings define_keys_bindings;
// ASCII char rendered to the right of each label. 0 = unassigned.
static char define_keys_bound_chars[OPTIONS_COUNT] = {0};

static uint8_t wait_release = 0u;

static void draw_define_keys_row(uint8_t index) __z88dk_fastcall
{
    // draw menu option
    const uint8_t attr = (index == menu_option_selected) ? MENU_ATTR_INVERSE : MENU_ATTR;
    draw_string(menu_option_rows[index], START_COL, attr, menu_option_labels[index]);

    // print selected key
    const char bound_char = define_keys_bound_chars[index];
    const char print_char = (bound_char == ' ' || bound_char == 0) ? '_' : bound_char;
    char print_str[2] = {print_char, '\0'};
    draw_string(menu_option_rows[index], BOUND_CHAR_COL, attr, print_str);
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
    wait_release = (in_inkey() == 0) ? 0u : 1u;

    // draw define keys menu
    for (uint8_t i = 0u; i < OPTIONS_COUNT; i++)
    {
        draw_define_keys_row(i);
    }
}

void define_keys_menu_scene_update(void) __z88dk_fastcall
{
    const uint8_t current_option = menu_option_selected;
    const int selected_key = in_inkey();

    // wait to release key
    if (selected_key == 0)
    {
        if (wait_release != 0u)
        {
            wait_release = 0u;
        }
        return;
    }
    if (wait_release != 0u)
    {
        return;
    }

    const uint16_t key_code = (uint16_t)in_key_scancode((unsigned char)selected_key);

    if (current_option == 0u)
    {
        define_keys_bindings.left = key_code;
    }
    else if (current_option == 1u)
    {
        define_keys_bindings.right = key_code;
    }
    else
    {
        define_keys_bindings.fire1 = key_code;
    }

    define_keys_bound_chars[current_option] = (char)selected_key;
    wait_release = 1u;

    // last slot: commit and bounce back to the main menu without advancing.
    if (current_option + 1u >= OPTIONS_COUNT)
    {
        draw_define_keys_row(current_option);
        input_set_keyboard_bindings(PLAYER_1, &define_keys_bindings);

        game_state = STATE_MENU;
    }
    else
    {
        select_define_keys_option(current_option + 1u);
    }
}
