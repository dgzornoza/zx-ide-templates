#include "input_manager.h"
#include <input.h>
#include <input/input_zx.h>

static udk_t keyboard_joystick_bindings;

static uint8_t left_pressed = 0;
static uint8_t right_pressed = 0;
static uint8_t up_pressed = 0;
static uint8_t down_pressed = 0;
static uint8_t input_initialized = 0;

static uint16_t keyboard_left_scancode = IN_KEY_SCANCODE_5;
static uint16_t keyboard_right_scancode = IN_KEY_SCANCODE_8;
static uint16_t keyboard_up_scancode = IN_KEY_SCANCODE_7;
static uint16_t keyboard_down_scancode = IN_KEY_SCANCODE_6;

static InputMode input_mode = INPUT_MODE_KEYBOARD_ONLY;
static InputJoystickType joystick_type = INPUT_JOYSTICK_KEMPSTON;

static uint16_t read_joystick_state(void)
{
    switch (joystick_type)
    {
    case INPUT_JOYSTICK_SINCLAIR1:
        return (uint16_t)in_stick_sinclair1();
    case INPUT_JOYSTICK_SINCLAIR2:
        return (uint16_t)in_stick_sinclair2();
    case INPUT_JOYSTICK_FULLER:
        return (uint16_t)in_stick_fuller();
    case INPUT_JOYSTICK_KEYBOARD:
        return (uint16_t)in_stick_keyboard(&keyboard_joystick_bindings);
    case INPUT_JOYSTICK_KEMPSTON:
    default:
        return (uint16_t)in_stick_kempston();
    }
}

void input_set_mode(InputMode mode)
{
    input_mode = mode;
}

void input_set_joystick_type(InputJoystickType new_joystick_type)
{
    joystick_type = new_joystick_type;
}

void input_set_keyboard_bindings(uint16_t left_scancode, uint16_t right_scancode, uint16_t up_scancode, uint16_t down_scancode)
{
    keyboard_left_scancode = left_scancode;
    keyboard_right_scancode = right_scancode;
    keyboard_up_scancode = up_scancode;
    keyboard_down_scancode = down_scancode;

    keyboard_joystick_bindings.left = left_scancode;
    keyboard_joystick_bindings.right = right_scancode;
    keyboard_joystick_bindings.up = up_scancode;
    keyboard_joystick_bindings.down = down_scancode;
}

void input_reset_defaults(void)
{
    input_mode = INPUT_MODE_KEYBOARD_ONLY;
    joystick_type = INPUT_JOYSTICK_KEMPSTON;
    input_set_keyboard_bindings(IN_KEY_SCANCODE_5, IN_KEY_SCANCODE_8, IN_KEY_SCANCODE_7, IN_KEY_SCANCODE_6);
    keyboard_joystick_bindings.fire = IN_KEY_SCANCODE_DISABLE;
}

void input_poll(void)
{
    uint16_t joystick_state = 0;
    uint8_t keyboard_left = 0;
    uint8_t keyboard_right = 0;
    uint8_t keyboard_up = 0;
    uint8_t keyboard_down = 0;

    if (!input_initialized)
    {
        input_reset_defaults();
        input_initialized = 1;
    }

    if (input_mode != INPUT_MODE_JOYSTICK_ONLY)
    {
        keyboard_left = (uint8_t)in_key_pressed(keyboard_left_scancode);
        keyboard_right = (uint8_t)in_key_pressed(keyboard_right_scancode);
        keyboard_up = (uint8_t)in_key_pressed(keyboard_up_scancode);
        keyboard_down = (uint8_t)in_key_pressed(keyboard_down_scancode);
    }

    if (input_mode != INPUT_MODE_KEYBOARD_ONLY)
    {
        joystick_state = read_joystick_state();
    }

    left_pressed = (uint8_t)(keyboard_left || (joystick_state & IN_STICK_LEFT));
    right_pressed = (uint8_t)(keyboard_right || (joystick_state & IN_STICK_RIGHT));
    up_pressed = (uint8_t)(keyboard_up || (joystick_state & IN_STICK_UP));
    down_pressed = (uint8_t)(keyboard_down || (joystick_state & IN_STICK_DOWN));
}

uint8_t is_left_pressed(void)
{
    return left_pressed;
}

uint8_t is_right_pressed(void)
{
    return right_pressed;
}

uint8_t is_up_pressed(void)
{
    return up_pressed;
}

uint8_t is_down_pressed(void)
{
    return down_pressed;
}
