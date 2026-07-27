#ifndef __CORE_SYSTEM_DEFINITIONS_H__
#define __CORE_SYSTEM_DEFINITIONS_H__

// -----------------------------------------------------------------------------
// System definitions
//
// Hardware-level constants for the ZX Spectrum (48K target). These are
// immutable properties of the target machine; do not override per-project.
//
// What does NOT belong here:
//   - Per-asset dimensions (HUD map size, sprite bounds)    → data/ header
//   - Per-scene UI layout (SCORE_X_POS, HUD_Y_OFFSET)       → scene .c
//   - Per-gameplay tuning (PLAYER_INITIAL_X, PLAYER_SPEED)  → entity .c
//   - Game-level configuration (player count, etc.)         → game_definitions.h
//   - Framework opt-in flags (FRAMEWORK_USE_*)              → own file when introduced
// -----------------------------------------------------------------------------

#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define CHAR_SIZE 8

#define SCREEN_CHARS_WIDTH (SCREEN_WIDTH / CHAR_SIZE)
#define SCREEN_CHARS_WIDTH_BASE_0 (SCREEN_WIDTH / CHAR_SIZE - 1)
#define SCREEN_CHARS_HEIGHT (SCREEN_HEIGHT / CHAR_SIZE)
#define SCREEN_CHARS_HEIGHT_BASE_0 (SCREEN_HEIGHT / CHAR_SIZE - 1)

#endif // __CORE_SYSTEM_DEFINITIONS_H__
