#ifndef __CORE_INPUT_INPUT_MANAGER_H__
#define __CORE_INPUT_INPUT_MANAGER_H__

#include <stdint.h>
#include "asm/input_keyboard_snapshot.h"
#include "../definitions.h"

/* INPUT_MAX_PLAYERS and PlayerId live in src/core/definitions.h
 * (framework configuration) so per-template customization happens in one place. */

/** Selects which input sources are evaluated each frame. */
typedef enum
{
    /** Read only keyboard bindings. */
    INPUT_MODE_KEYBOARD_ONLY,
    /** Read only configured joystick device. */
    INPUT_MODE_JOYSTICK_ONLY,
    /** Read keyboard and joystick and merge both states. */
    INPUT_MODE_KEYBOARD_AND_JOYSTICK
} InputMode;

/** Selects which joystick backend is used by the input manager. */
typedef enum
{
    /** Kempston joystick interface. */
    INPUT_JOYSTICK_KEMPSTON,
    /** Sinclair Interface II, player 1 mapping. */
    INPUT_JOYSTICK_SINCLAIR1,
    /** Sinclair Interface II, player 2 mapping. */
    INPUT_JOYSTICK_SINCLAIR2,
    /** Fuller joystick interface. */
    INPUT_JOYSTICK_FULLER
} InputJoystickType;

/** Keyboard scancode bindings for all polled actions. */
typedef struct
{
    uint16_t left;
    uint16_t right;
    uint16_t up;
    uint16_t down;
    uint16_t fire1;
} InputBindings;

/** Bit flags returned by input_get_pressed(). AND with the relevant flag to test an action. */
typedef enum
{
    INPUT_FLAG_LEFT = (1u << 0),
    INPUT_FLAG_RIGHT = (1u << 1),
    INPUT_FLAG_UP = (1u << 2),
    INPUT_FLAG_DOWN = (1u << 3),
    INPUT_FLAG_FIRE1 = (1u << 4)
} InputFlags;

/** Per-player input state. Sized exactly for sdcc_iy; verify via .map if refactored. */
typedef struct
{
    InputBindings bindings; /* offset 0;  10 B */
    uint8_t pressed;        /* offset 10; InputFlags bitmask */
    uint8_t mode;           /* offset 11; raw InputMode (uint8_t, NOT typed enum) */
    uint8_t joystick_type;  /* offset 12; raw InputJoystickType (uint8_t, NOT typed enum) */
} InputPlayerState;         /* sizeof = 13 */

/**
 * Per-player poll: combines the cached keyboard state for that player's
 * bindings with the player's joystick (if mode allows) and updates
 * players[player].pressed. ZERO port I/O after input_keyboard_snapshot()
 * has been called this frame.
 */
void input_poll(PlayerId player) __z88dk_fastcall;

/**
 * Returns the InputFlags bitmask for the given player. Read once per frame
 * and AND each flag in your consumer.
 */
uint8_t input_get_pressed(uint8_t player) __z88dk_fastcall;

/**
 * Restores the given player to its default bindings (shared between players),
 * mode (KEYBOARD_ONLY), and per-player default joystick type. Call once per
 * player from the owning scene's init. The default joysticks are P0=KEMPSTON,
 * P1=SINCLAIR2; the keyboard bindings are shared (left=5, right=8, up=7,
 * down=6, fire1=SPACE).
 */
void input_reset_defaults(uint8_t player) __z88dk_fastcall;

/**
 * Returns 1 if the given scancode is currently pressed per keyboard_cache,
 * 0 otherwise. Mirrors z88dk asm_in_key_pressed bit-for-bit; zero port I/O.
 * Useful for reading arbitrary system keys (pause, menu, etc.) without
 * re-touching hardware.
 */
uint8_t input_keyboard_pressed(uint16_t scancode) __z88dk_fastcall;

/**
 * Sets which input sources are evaluated each frame for the given player.
 * Two args; uses default callee convention (NOT fastcall).
 */
void input_set_mode(uint8_t player, InputMode mode);

/**
 * Selects the joystick backend used when joystick input is enabled for the
 * given player. Two args; default callee convention.
 */
void input_set_joystick_type(uint8_t player, InputJoystickType joystick_type);

/**
 * Sets keyboard scancodes for all actions for the given player from the
 * given struct. Pointer-passed because the 10-B struct must never cross a
 * call boundary by value on the ~128 B stack budget.
 */
void input_set_keyboard_bindings(uint8_t player, const InputBindings *bindings);

#endif // __CORE_INPUT_INPUT_MANAGER_H__
