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
    INPUT_JOYSTICK_FULLER,
    /** Keyboard-simulated joystick using current key bindings. */
    INPUT_JOYSTICK_KEYBOARD
} InputJoystickType;

/**
 * Polls current input state and updates directional pressed flags.
 * Call once per frame before gameplay update.
 */
void input_poll(void);

/** Sets active input mode (keyboard, joystick, or both). */
void input_set_mode(InputMode mode);

/** Selects joystick backend used when joystick input is enabled. */
void input_set_joystick_type(InputJoystickType joystick_type);

/**
 * Sets keyboard scancodes for left, right, up and down actions.
 * These bindings are also reused when joystick type is keyboard.
 */
void input_set_keyboard_bindings(uint16_t left_scancode, uint16_t right_scancode, uint16_t up_scancode, uint16_t down_scancode);

/** Restores default mode, joystick type and keyboard bindings. */
void input_reset_defaults(void);

/** Returns whether the left input is currently pressed. */
uint8_t is_left_pressed(void);
/** Returns whether the right input is currently pressed. */
uint8_t is_right_pressed(void);
/** Returns whether the up input is currently pressed. */
uint8_t is_up_pressed(void);
/** Returns whether the down input is currently pressed. */
uint8_t is_down_pressed(void);

#endif // __CORE_INPUT_INPUT_MANAGER_H__
