#ifndef __CORE_GAME_DEFINITIONS_H__
#define __CORE_GAME_DEFINITIONS_H__

// -----------------------------------------------------------------------------
// Game definitions
//
// Game-level compile-time configuration. These values affect the API surface
// of multiple modules (input, scenes, entities) and must be in a single
// source of truth.
//
// What does NOT belong here:
//   - Hardware constants (screen size, char size)  → system_definitions.h
//   - Per-asset dimensions, per-scene layout        → consumer .c files
// -----------------------------------------------------------------------------

// --- Game-level constants ---

// ALLOWED_GAME_PLAYERS: number of allowed simultaneous players.
// This is a game-level concept: how many players the game supports. The input
// manager sizes its player array from this value.
#ifndef ALLOWED_GAME_PLAYERS
#define ALLOWED_GAME_PLAYERS 1
#endif

#if (ALLOWED_GAME_PLAYERS < 1) || (ALLOWED_GAME_PLAYERS > 2)
#error "ALLOWED_GAME_PLAYERS must be 1 or 2 currently"
#endif

/**
 * Player identifier type.
 * This is used to identify players in game.
 */
typedef enum
{
    PLAYER_1 = 0,
    PLAYER_2 = 1
} PlayerId;

#endif // __CORE_GAME_DEFINITIONS_H__
