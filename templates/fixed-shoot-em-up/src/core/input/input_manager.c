#include "input_manager.h"
#include "../game_definitions.h"
#include <input.h>
#include <input/input_zx.h>

/**
 * Per-player state. BSS-zero at boot, so a player that has not been reset reads pressed=0.
 */
static InputPlayerState players[ALLOWED_GAME_PLAYERS];

/** Shared default keyboard bindings. */
static const InputBindings default_bindings = {
    IN_KEY_SCANCODE_o,
    IN_KEY_SCANCODE_p,
    IN_KEY_SCANCODE_q,
    IN_KEY_SCANCODE_a,
    IN_KEY_SCANCODE_SPACE};

/** Per-player default input. P0=keyboard, P1=kempston. */
static const InputType default_input[ALLOWED_GAME_PLAYERS] = {
    INPUT_KEYBOARD
#if ALLOWED_GAME_PLAYERS == 2
    ,
    INPUT_JOYSTICK_KEMPSTON
#endif
};

/**
 * Reads the joystick state for the given player via the per-player
 * input_type selector. Switch dispatches to the z88dk backend.
 */
static uint16_t read_joystick_state(PlayerId player) __z88dk_fastcall
{
    switch (players[player].input_type)
    {
    case INPUT_JOYSTICK_SINCLAIR:
        // player one, use sinclair I, else sinclair II, should be change in a future
        if (player == PLAYER_1)
        {
            return (uint16_t)in_stick_sinclair1();
        }
        else
        {
            return (uint16_t)in_stick_sinclair2();
        }
    case INPUT_JOYSTICK_FULLER:
        return (uint16_t)in_stick_fuller();
    case INPUT_JOYSTICK_KEMPSTON:
    default:
        return (uint16_t)in_stick_kempston();
    }
}

void input_poll(PlayerId playerId) __z88dk_fastcall
{
    uint8_t flags = 0;

    if (players[playerId].input_type == INPUT_KEYBOARD)
    {
        const InputBindings *bindings = &players[playerId].bindings;
#if (GAME_USED_KEYS & GAME_KEY_LEFT_BIT)
        if (in_key_pressed(bindings->left))
        {
            flags |= INPUT_FLAG_LEFT;
        }
#endif
#if (GAME_USED_KEYS & GAME_KEY_RIGHT_BIT)
        if (in_key_pressed(bindings->right))
        {
            flags |= INPUT_FLAG_RIGHT;
        }
#endif
#if (GAME_USED_KEYS & GAME_KEY_UP_BIT)
        if (in_key_pressed(bindings->up))
        {
            flags |= INPUT_FLAG_UP;
        }
#endif
#if (GAME_USED_KEYS & GAME_KEY_DOWN_BIT)
        if (in_key_pressed(bindings->down))
        {
            flags |= INPUT_FLAG_DOWN;
        }
#endif
#if (GAME_USED_KEYS & GAME_KEY_FIRE1_BIT)
        if (in_key_pressed(bindings->fire1))
        {
            flags |= INPUT_FLAG_FIRE1;
        }
#endif
    }
    else
    {
        const uint16_t joystick_state = read_joystick_state(playerId);
#if (GAME_USED_KEYS & GAME_KEY_LEFT_BIT)
        if (joystick_state & IN_STICK_LEFT)
        {
            flags |= INPUT_FLAG_LEFT;
        }
#endif
#if (GAME_USED_KEYS & GAME_KEY_RIGHT_BIT)
        if (joystick_state & IN_STICK_RIGHT)
        {
            flags |= INPUT_FLAG_RIGHT;
        }
#endif
#if (GAME_USED_KEYS & GAME_KEY_UP_BIT)
        if (joystick_state & IN_STICK_UP)
        {
            flags |= INPUT_FLAG_UP;
        }
#endif
#if (GAME_USED_KEYS & GAME_KEY_DOWN_BIT)
        if (joystick_state & IN_STICK_DOWN)
        {
            flags |= INPUT_FLAG_DOWN;
        }
#endif
#if (GAME_USED_KEYS & GAME_KEY_FIRE1_BIT)
        if (joystick_state & IN_STICK_FIRE)
        {
            flags |= INPUT_FLAG_FIRE1;
        }
#endif
    }

    players[playerId].pressed = flags;
}

uint8_t input_get_pressed(PlayerId player) __z88dk_fastcall
{
    return players[player].pressed;
}

void input_reset_defaults(PlayerId player) __z88dk_fastcall
{
    players[player].bindings = default_bindings;
    players[player].input_type = default_input[player];
}

void input_set_input_type(PlayerId player, InputType input_type)
{
    players[player].input_type = input_type;
}

void input_set_keyboard_bindings(PlayerId player, const InputBindings *bindings)
{
    players[player].bindings = *bindings;
}

uint8_t input_keyboard_pressed(uint16_t scancode) __z88dk_fastcall
{
    return (uint8_t)in_key_pressed(scancode);
}
