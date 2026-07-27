#ifndef __SCENES_FEATURES_ENTITIES_PLAYER_H__
#define __SCENES_FEATURES_ENTITIES_PLAYER_H__

#include <stdint.h>
#include "../../../core/input/input_manager.h"

/**
 * Initializes the player entity. Must be called exactly once per player from the
 * owning scene's init, BEFORE the first player_update() / player_render()
 * in the same scene.
 *
 * @param player_id PlayerId (PLAYER_1 or PLAYER_2).
 */
void player_init(uint8_t player_id) __z88dk_fastcall;

/**
 * Per-frame update: reads the player's input flags from the input manager,
 * applies the movement logic, and updates the internal sprite position.
 * Call ONCE per player per frame from the owning scene's update(),
 * AFTER input_poll(player_id) has run for the same player in the same frame and AFTER input_keyboard_snapshot()
 * has refreshed the keyboard cache for the frame.
 *
 * @param player_id PlayerId (PLAYER_1 or PLAYER_2).
 */
void player_update(uint8_t player_id) __z88dk_fastcall;

/**
 * Repaints the player sprite at the current (x, y),
 * clipped by player_clip. Call ONCE per player per frame from the owning
 * scene's render()
 *
 * @param player_id PlayerId (PLAYER_1 or PLAYER_2).
 */
void player_render(uint8_t player_id) __z88dk_fastcall;

#endif // __SCENES_FEATURES_ENTITIES_PLAYER_H__
