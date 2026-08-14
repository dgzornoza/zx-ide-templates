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

// SCREEN_COLOR: game screen color
#ifndef SCREEN_COLOR
#define SCREEN_COLOR INK_WHITE | PAPER_BLACK
#endif

// MENU ATTRIBUTES
#ifndef MENU_ATTR
#define MENU_ATTR (INK_WHITE | PAPER_BLACK)
#endif

#ifndef MENU_ATTR_INVERSE
#define MENU_ATTR_INVERSE (INK_BLACK | PAPER_WHITE)
#endif

// ALLOWED_GAME_PLAYERS: number of allowed simultaneous players.
// This is a game-level concept: how many players the game supports. The input
// manager sizes its player array from this value.
#ifndef ALLOWED_GAME_PLAYERS
#define ALLOWED_GAME_PLAYERS 1
#endif

#if (ALLOWED_GAME_PLAYERS < 1) || (ALLOWED_GAME_PLAYERS > 2)
#error "ALLOWED_GAME_PLAYERS must be 1 or 2 currently"
#endif

// GAME USED KEYS
/**
 * Compile-time bitmask of which input actions are emitted into input_poll.
 * Bit positions are exposed as GAME_KEY_*_BIT macros so the preprocessor
 * can AND them at translation time (sdcc does not expand enum members
 * inside #if). The macros mirror the InputFlags enum in
 * core/input/input_manager.h; both must stay in lockstep.
 * Default keeps every action live. Apps override BEFORE any core include
 * to slim down the compiled input_poll, e.g.:
 *   #define GAME_USED_KEYS (GAME_KEY_LEFT_BIT | GAME_KEY_RIGHT_BIT | GAME_KEY_FIRE1_BIT)
 */
#define GAME_KEY_LEFT_BIT (1u << 0)
#define GAME_KEY_RIGHT_BIT (1u << 1)
#define GAME_KEY_UP_BIT (1u << 2)
#define GAME_KEY_DOWN_BIT (1u << 3)
#define GAME_KEY_FIRE1_BIT (1u << 4)

#ifndef GAME_USED_KEYS
#define GAME_USED_KEYS (GAME_KEY_LEFT_BIT | GAME_KEY_RIGHT_BIT | GAME_KEY_UP_BIT | GAME_KEY_DOWN_BIT | GAME_KEY_FIRE1_BIT)
#endif

// PLAYERS

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
