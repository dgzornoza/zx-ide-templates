#include "input_manager.h"
#include <input.h>
#include <input/input_zx.h>

InputBindings input_bindings;

static const InputBindings default_bindings = {
    IN_KEY_SCANCODE_5,
    IN_KEY_SCANCODE_8,
    IN_KEY_SCANCODE_7,
    IN_KEY_SCANCODE_6,
    IN_KEY_SCANCODE_SPACE};

static uint8_t pressed_flags = 0;
static uint8_t input_initialized = 0;

static InputMode input_mode = INPUT_MODE_KEYBOARD_ONLY;
static InputJoystickType joystick_type = INPUT_JOYSTICK_KEMPSTON;

static uint16_t read_joystick_state(void) __z88dk_fastcall
{
    switch (joystick_type)
    {
    case INPUT_JOYSTICK_SINCLAIR1:
        return (uint16_t)in_stick_sinclair1();
    case INPUT_JOYSTICK_SINCLAIR2:
        return (uint16_t)in_stick_sinclair2();
    case INPUT_JOYSTICK_FULLER:
        return (uint16_t)in_stick_fuller();
    case INPUT_JOYSTICK_KEMPSTON:
    default:
        return (uint16_t)in_stick_kempston();
    }
}

void input_set_mode(InputMode mode) __z88dk_fastcall
{
    input_mode = mode;
}

void input_set_joystick_type(InputJoystickType new_joystick_type) __z88dk_fastcall
{
    joystick_type = new_joystick_type;
}

void input_set_keyboard_bindings(const InputBindings *new_bindings) __z88dk_fastcall
{
    input_bindings = *new_bindings;
}

void input_reset_defaults(void) __z88dk_fastcall
{
    input_mode = INPUT_MODE_KEYBOARD_ONLY;
    joystick_type = INPUT_JOYSTICK_KEMPSTON;
    input_set_keyboard_bindings(&default_bindings);
}

void input_poll(void) __z88dk_fastcall
{
    uint16_t joystick_state = 0;
    uint8_t flags = 0;

    if (!input_initialized)
    {
        input_reset_defaults();
        input_initialized = 1;
    }

    if (input_mode != INPUT_MODE_JOYSTICK_ONLY)
    {
        if (in_key_pressed(input_bindings.left))
        {
            flags |= INPUT_FLAG_LEFT;
        }
        if (in_key_pressed(input_bindings.right))
        {
            flags |= INPUT_FLAG_RIGHT;
        }
        if (in_key_pressed(input_bindings.up))
        {
            flags |= INPUT_FLAG_UP;
        }
        if (in_key_pressed(input_bindings.down))
        {
            flags |= INPUT_FLAG_DOWN;
        }
        if (in_key_pressed(input_bindings.fire1))
        {
            flags |= INPUT_FLAG_FIRE1;
        }
    }

    if (input_mode != INPUT_MODE_KEYBOARD_ONLY)
    {
        joystick_state = read_joystick_state();
        if (joystick_state & IN_STICK_LEFT)
        {
            flags |= INPUT_FLAG_LEFT;
        }
        if (joystick_state & IN_STICK_RIGHT)
        {
            flags |= INPUT_FLAG_RIGHT;
        }
        if (joystick_state & IN_STICK_UP)
        {
            flags |= INPUT_FLAG_UP;
        }
        if (joystick_state & IN_STICK_DOWN)
        {
            flags |= INPUT_FLAG_DOWN;
        }
        if (joystick_state & IN_STICK_FIRE)
        {
            flags |= INPUT_FLAG_FIRE1;
        }
    }

    pressed_flags = flags;
}

uint8_t input_get_pressed(void) __z88dk_fastcall
{
    return pressed_flags;
}
