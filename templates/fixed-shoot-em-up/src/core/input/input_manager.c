#include "input_manager.h"
#include <input.h>
#include <input/input_zx.h>

/**
 * Cache of 8 inverted half-row reads, populated each frame by the snapshot
 * ASM in asm/input_keyboard_snapshot.asm. The ASM `EXTERN`s this symbol.
 */
uint8_t keyboard_cache[8];

/**
 * Per-player state. 26 B BSS (2 x 13 B). BSS-zero at boot, so a player that
 * has not been reset reads pressed=0 (REQ-006 first-frame tie-break holds).
 */
static InputPlayerState players[INPUT_MAX_PLAYERS];

/** Shared default keyboard bindings; one copy in ROM (REQ-009). */
static const InputBindings default_bindings = {
    IN_KEY_SCANCODE_5,
    IN_KEY_SCANCODE_8,
    IN_KEY_SCANCODE_7,
    IN_KEY_SCANCODE_6,
    IN_KEY_SCANCODE_SPACE};

/** Per-player default joystick backend. P0=Kempston, P1=Sinclair2 (if MAX >= 2). */
static const uint8_t default_joy[INPUT_MAX_PLAYERS] = {
    INPUT_JOYSTICK_KEMPSTON
#if INPUT_MAX_PLAYERS >= 2
    , INPUT_JOYSTICK_SINCLAIR2
#endif
};

/**
 * Resolves the half-row index for a row-selector byte. The Spectrum returns
 * the active-low 5 bits for the row whose bit is clear in the selector.
 * The selector has exactly one bit = 0; its position is the row index.
 * Switch over the 8 valid selectors; default returns 0 (DEFS safety).
 */
static uint8_t row_low_to_index(uint8_t l)
{
    switch (l)
    {
    case 0xfe:
        return 0;
    case 0xfd:
        return 1;
    case 0xfb:
        return 2;
    case 0xf7:
        return 3;
    case 0xef:
        return 4;
    case 0xdf:
        return 5;
    case 0xbf:
        return 6;
    case 0x7f:
        return 7;
    default:
        return 0;
    }
}

/**
 * Test one scancode against the live keyboard_cache. Mirrors
 * asm_in_key_pressed bit-for-bit; zero port I/O.
 *  H: flags + low-5-bit key mask
 *  L: row selector -> half-row index in keyboard_cache
 */
static uint8_t cache_test_scancode(uint16_t scancode) __z88dk_fastcall
{
    const uint8_t h = (uint8_t)(scancode >> 8);
    const uint8_t l = (uint8_t)(scancode & 0xff);
    /* Caps shift required? (row 0, bit 0) */
    if ((h & 0x80) && !(keyboard_cache[0] & 0x01))
    {
        return 0;
    }
    /* Sym shift required? (row 7, bit 1) */
    if ((h & 0x40) && !(keyboard_cache[7] & 0x02))
    {
        return 0;
    }
    const uint8_t row_index = row_low_to_index(l);
    return (keyboard_cache[row_index] & (uint8_t)(h & 0x1f)) ? 1 : 0;
}

/**
 * Reads the joystick state for the given player via the per-player
 * joystick_type selector. Switch dispatches to the z88dk backend.
 */
static uint16_t read_joystick_state(uint8_t player) __z88dk_fastcall
{
    switch ((InputJoystickType)players[player].joystick_type)
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

/**
 * Poll one player: combine cached keyboard state for that player's bindings
 * with the joystick (if mode allows), and store the resulting InputFlags.
 * ZERO port $FE reads; ZERO port-joystick reads unless joystick is enabled.
 */
void input_poll(PlayerId player) __z88dk_fastcall
{
    uint8_t flags = 0;
    const InputPlayerState *p = &players[player];
    const InputMode mode = (InputMode)p->mode;

    if (mode != INPUT_MODE_JOYSTICK_ONLY)
    {
        if (cache_test_scancode(p->bindings.left))
        {
            flags |= INPUT_FLAG_LEFT;
        }
        if (cache_test_scancode(p->bindings.right))
        {
            flags |= INPUT_FLAG_RIGHT;
        }
        if (cache_test_scancode(p->bindings.up))
        {
            flags |= INPUT_FLAG_UP;
        }
        if (cache_test_scancode(p->bindings.down))
        {
            flags |= INPUT_FLAG_DOWN;
        }
        if (cache_test_scancode(p->bindings.fire1))
        {
            flags |= INPUT_FLAG_FIRE1;
        }
    }

    if (mode != INPUT_MODE_KEYBOARD_ONLY)
    {
        const uint16_t joystick_state = read_joystick_state(player);
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

    players[player].pressed = flags;
}

uint8_t input_get_pressed(uint8_t player) __z88dk_fastcall
{
    return players[player].pressed;
}

/**
 * Per-player reset (REQ decision #3): bind shared default keyboard, mode =
 * KEYBOARD_ONLY, joystick_type = default_joy[player] (P0=Kempston, P1=Sinclair2).
 */
void input_reset_defaults(uint8_t player) __z88dk_fastcall
{
    players[player].bindings = default_bindings;
    players[player].mode = (uint8_t)INPUT_MODE_KEYBOARD_ONLY;
    players[player].joystick_type = default_joy[player];
}

/** Mirrors asm_in_key_pressed; zero port I/O. */
uint8_t input_keyboard_pressed(uint16_t scancode) __z88dk_fastcall
{
    return cache_test_scancode(scancode);
}

void input_set_mode(uint8_t player, InputMode mode)
{
    players[player].mode = (uint8_t)mode;
}

void input_set_joystick_type(uint8_t player, InputJoystickType joystick_type)
{
    players[player].joystick_type = (uint8_t)joystick_type;
}

void input_set_keyboard_bindings(uint8_t player, const InputBindings *bindings)
{
    players[player].bindings = *bindings;
}
