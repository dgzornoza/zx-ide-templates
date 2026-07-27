// -----------------------------------------------------------------------------
// System defined constants

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define CHAR_SIZE 8

#define SCREEN_CHARS_WIDTH (SCREEN_WIDTH / CHAR_SIZE)
#define SCREEN_CHARS_WIDTH_BASE_0 (SCREEN_WIDTH / CHAR_SIZE - 1)
#define SCREEN_CHARS_HEIGHT (SCREEN_HEIGHT / CHAR_SIZE)
#define SCREEN_CHARS_HEIGHT_BASE_0 (SCREEN_HEIGHT / CHAR_SIZE - 1)

// -----------------------------------------------------------------------------
// Framework defined constants
//
// Single source of truth for framework-wide compile-time configuration.
// Three categories live here, in order:
//   1. Feature flags     — opt in/out of optional framework modules.
//   2. Per-template      — values like INPUT_MAX_PLAYERS that affect memory
//      layout and API; validated at compile time so misconfiguration fails.
//   3. Player identifiers — PlayerId sized to INPUT_MAX_PLAYERS.
//
// What does NOT belong here:
//   - Per-asset dimensions (HUD map size, sprite bounds)     → data/ header
//   - Per-scene UI layout (SCORE_X_POS, HUD_Y_OFFSET)       → scene .c
//   - Per-gameplay tuning (PLAYER_INITIAL_X, PLAYER_SPEED)  → entity .c
//   - Hard-coded ZX values (attribute format, IM2 layout)   → System above

// --- 1. Feature flags (compile in/out of optional modules) ---
//
// Pattern (no flags defined yet; the first one appears with its module):
//   #ifndef FRAMEWORK_USE_<NAME>
//   #define FRAMEWORK_USE_<NAME> <default>
//   #endif
// Override per-project via the Makefile (-DFRAMEWORK_USE_<NAME>=0) or by
// editing the default below.

// --- 2. Per-template constants ---

// INPUT_MAX_PLAYERS: number of simultaneous players (1 or 2 on the 48K target).
// Setting to 1 saves 13 B of BSS (one InputPlayerState) and compiles out
// second-player code paths in any scene that gates with #if INPUT_MAX_PLAYERS >= 2.
#ifndef INPUT_MAX_PLAYERS
#define INPUT_MAX_PLAYERS 1
#endif

#if (INPUT_MAX_PLAYERS < 1) || (INPUT_MAX_PLAYERS > 2)
#error "INPUT_MAX_PLAYERS must be 1 or 2 on the current 48K target"
#endif

// --- 3. Player identifiers (sized to INPUT_MAX_PLAYERS) ---
//
// PLAYER_1 is always index 0. PLAYER_2 exists only when INPUT_MAX_PLAYERS >= 2
// so single-player templates that reference PLAYER_2 fail to compile.
#if INPUT_MAX_PLAYERS >= 2
typedef enum
{
    PLAYER_1 = 0,
    PLAYER_2 = 1
} PlayerId;
#else
typedef enum
{
    PLAYER_1 = 0
} PlayerId;
#endif
