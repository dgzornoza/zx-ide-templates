#ifndef __CORE_INPUT_INPUT_MANAGER_H__
#define __CORE_INPUT_INPUT_MANAGER_H__

#include <stdint.h>
#include "../game_definitions.h"

/** Selects which joystick backend is used by the input manager. */
typedef enum
{
    /** Keyboard */
    INPUT_KEYBOARD,
    /** Kempston joystick interface. */
    INPUT_JOYSTICK_KEMPSTON,
    /** Sinclair Interface II. */
    INPUT_JOYSTICK_SINCLAIR,
    /** Fuller joystick interface. */
    INPUT_JOYSTICK_FULLER
} InputType;

/** Bit flags to test an action. */
typedef enum
{
    INPUT_FLAG_LEFT = (1u << 0),
    INPUT_FLAG_RIGHT = (1u << 1),
    INPUT_FLAG_UP = (1u << 2),
    INPUT_FLAG_DOWN = (1u << 3),
    INPUT_FLAG_FIRE1 = (1u << 4)
} InputFlags;

/** Keyboard scancode bindings for all polled actions. */
typedef struct
{
    uint16_t left;
    uint16_t right;
    uint16_t up;
    uint16_t down;
    uint16_t fire1;
} InputBindings;

/** Per-player input state.
 * Field order is chosen for the hottest ROM access */
typedef struct
{
    InputFlags pressed;     /* offset 0;  1 B; InputFlags bitmask */
    InputBindings bindings; /* offset 1;  10 B */
    InputType input_type;   /* offset 11; raw InputType */
} InputPlayerState;

/**
 * Per-player poll: tests each binding against the keyboard
 * Then stores in player for one frame. This action performs multiples input port read.
 * It should be called once at the beginning of the main loop
 */
void input_poll(PlayerId player) __z88dk_fastcall;

/**
 * Returns the InputFlags bitmask for the given player. (from input_poll)
 */
uint8_t input_get_pressed(PlayerId player) __z88dk_fastcall;

/**
 * Restores the given player to its default input bindings. Call once per
 * player from the input menu selection.
 * Default keyboard bindings are shared (left=o, right=p, up=q, down=a, fire1=SPACE).
 */
void input_reset_defaults(PlayerId player) __z88dk_fastcall;

/**
 * Selects the input type for the given player.
 */
void input_set_input_type(PlayerId player, InputType input_type);

/**
 * Define Keys for player.
 * Sets keyboard scancodes for all actions for the given player from the given struct.
 */
void input_set_keyboard_bindings(PlayerId player, const InputBindings *bindings);

/**
 * Returns 1 if the given scancode is currently pressed, 0 otherwise.
 * This method performs one input port read.
 */
uint8_t input_keyboard_pressed(uint16_t scancode) __z88dk_fastcall;

#endif // __CORE_INPUT_INPUT_MANAGER_H__
