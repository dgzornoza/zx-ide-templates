#ifndef __SCENES_FEATURES_ENTITIES_PLAYER_H__
#define __SCENES_FEATURES_ENTITIES_PLAYER_H__

#include <stdint.h>
#include "../../../core/input/input_manager.h"

/**
 * Initializes the player entity. Must be called exactly once per player from the
 * owning scene's init, BEFORE the first player_update() in the same scene.
 *
 * @param player_id PlayerId (PLAYER_1 or PLAYER_2).
 */
void player_init(uint8_t player_id) __z88dk_fastcall;

/**
 * Per-frame tick: should reads the player's input flags from the input manager and
 * applies movement logic.
 *
 * Call ONCE per player per frame from the owning scene's update(), AFTER
 * input_poll(player_id) has run for the same player in the same frame.
 *
 * @param player_id PlayerId (PLAYER_1 or PLAYER_2).
 */
void player_update(uint8_t player_id) __z88dk_fastcall;

#endif // __SCENES_FEATURES_ENTITIES_PLAYER_H__
