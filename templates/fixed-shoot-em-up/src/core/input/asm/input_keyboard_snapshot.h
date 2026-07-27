#ifndef __CORE_INPUT_INPUT_KEYBOARD_SNAPSHOT_H__
#define __CORE_INPUT_INPUT_KEYBOARD_SNAPSHOT_H__

#include <stdint.h>

/**
 * 8-byte cache of inverted half-row reads from port $FE (one byte per
 * half-row, low 5 bits, bit=1 means the corresponding key is held).
 */
extern uint8_t keyboard_cache[8];

/**
 * Captures the current state of the ZX Spectrum keyboard matrix in a single
 * 8-IN sweep of port $FE, populating `keyboard_cache[8]`.
 *
 * Implemented to keep the snapshot to a single hardware-side scan per frame.
 * Callers MUST invoke this exactly once per frame, before any per-player input_poll().
 *
 * Clobbers: AF, BC, DE.
 * Stack depth: unchanged at call site (no args, no saved registers).
 */
extern void input_keyboard_snapshot(void) __z88dk_fastcall;

#endif // __CORE_INPUT_INPUT_KEYBOARD_SNAPSHOT_H__
