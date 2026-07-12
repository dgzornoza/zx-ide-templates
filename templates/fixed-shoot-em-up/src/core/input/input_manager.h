#ifndef __CORE_INPUT_INPUT_MANAGER_H__
#define __CORE_INPUT_INPUT_MANAGER_H__

#include <stdint.h>

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

/** Global binding state. Read each frame by input_poll(). */
extern InputBindings input_bindings;

/**
 * Polls current input state and updates the pressed-flags bitmask.
 * Call once per frame before gameplay update.
 */
void input_poll(void) __z88dk_fastcall;

/**
 * Returns the bitmask of actions currently pressed.
 * Combine with InputFlags using bitwise AND: `(input_get_pressed() & INPUT_FLAG_LEFT)`.
 * Reading once per frame and testing multiple flags is cheaper than calling a getter per action.
 */
uint8_t input_get_pressed(void) __z88dk_fastcall;

/** Sets active input mode (keyboard, joystick, or both). */
void input_set_mode(InputMode mode) __z88dk_fastcall;

/** Selects joystick backend used when joystick input is enabled. */
void input_set_joystick_type(InputJoystickType joystick_type) __z88dk_fastcall;

/**
 * Sets keyboard scancodes for all actions from the given struct.
 * Also keeps the keyboard-as-joystick bindings in sync.
 */
void input_set_keyboard_bindings(const InputBindings *bindings) __z88dk_fastcall;

/** Restores default mode, joystick type and keyboard bindings. */
void input_reset_defaults(void) __z88dk_fastcall;

#endif // __CORE_INPUT_INPUT_MANAGER_H__
