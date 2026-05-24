#ifndef __CORE_INPUT_KEYS_MANAGER_H__
#define __CORE_INPUT_KEYS_MANAGER_H__

#include <stdint.h>

/**
 * Polls the current state of the input devices and updates the internal state accordingly.
 */
void keys_input_poll(void);

/** Returns whether the left input is currently pressed. */
uint8_t is_left_pressed(void);
/** Returns whether the right input is currently pressed. */
uint8_t is_right_pressed(void);
/** Returns whether the up input is currently pressed. */
uint8_t is_up_pressed(void);
/** Returns whether the down input is currently pressed. */
uint8_t is_down_pressed(void);

#endif // __CORE_INPUT_KEYS_MANAGER_H__
