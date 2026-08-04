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

// Synchronization contract:
//   Each #define below has a matching EQU in
//   src/core/system_definitions.asm. Z80 assembly cannot #include this
//   header (no preprocessor in the asm toolchain), so the values are
//   duplicated there as EQU constants and pulled into each consumer .asm.
//   If you change a value here, update the .asm too; the preprocessor will not catch a mismatch.
//
// Scope rule: this header MUST contain ONLY #define constants and
// pure-C declarations. No executable code, no inline asm, no .asm/.c
// bodies.

#define SYS_TICKS_PER_SECOND 50
#define SYS_SCREEN_WIDTH 256
#define SYS_SCREEN_HEIGHT 192
#define SYS_CHAR_SIZE 8

#define SYS_SCREEN_CHARS_WIDTH (SYS_SCREEN_WIDTH / SYS_CHAR_SIZE)
#define SYS_SCREEN_CHARS_WIDTH_BASE_0 (SYS_SCREEN_WIDTH / SYS_CHAR_SIZE - 1)
#define SYS_SCREEN_CHARS_HEIGHT (SYS_SCREEN_HEIGHT / SYS_CHAR_SIZE)
#define SYS_SCREEN_CHARS_HEIGHT_BASE_0 (SYS_SCREEN_HEIGHT / SYS_CHAR_SIZE - 1)

#endif // __CORE_SYSTEM_DEFINITIONS_H__
