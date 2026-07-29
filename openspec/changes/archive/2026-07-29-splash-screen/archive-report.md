# Archive Report: Splash Screen

## SDD Cycle Complete

**Change**: `splash-screen`
**Archived to**: `openspec/changes/archive/2026-07-29-splash-screen/`
**Final Verdict**: PASS WITH WARNINGS (0 CRITICAL, 1 WARNING, 3 SUGGESTIONS)

## Specs Synced

| Domain | Action | Details |
|--------|--------|---------|
| `boot-splash` | Created (no prior canonical) | 5 requirements / 11 scenarios. Source of truth at `openspec/specs/boot-splash/spec.md`. |
| `game-state-machine` | Created (no prior canonical) | 4 requirements / 8 scenarios. Source of truth at `openspec/specs/game-state-machine/spec.md`. |

(No delta specs in the change folder — both specs were written directly to canonical at sdd-spec time because no prior capability existed.)

## Archive Contents

- `proposal.md` ✓ (449 words, under 450 budget)
- `design.md` ✓ (798 words, under 800 budget)
- `tasks.md` ✓ (528 words, under 530 budget; 18/18 tasks checked)
- `apply-progress.md` ✓ (per-task status; build PASS evidence; post-apply include corrections documented)
- `verify-report.md` ✓ (PASS WITH WARNINGS; 17/19 scenarios PASS, 2 DEFERRED-MANUAL for emulator)

## Source of Truth Updated

| Spec | Path |
|------|------|
| `boot-splash` | `openspec/specs/boot-splash/spec.md` |
| `game-state-machine` | `openspec/specs/game-state-machine/spec.md` |

## Implementation Summary

- 6 files changed (3 modified `main.c`, `game_state.{h,c}`, `Readme.md`; 2 created `splash.{h,c}`).
- splash.c: 119 → 55 lines (~80% reduction). Hand-encoded glyph table deleted; SP1 `font_1` tile registry reused.
- `sp1_Initialize(MAKE_ROTTBL)` (NO `OVERWRITE_DFILE`/`OVERWRITE_TILES`); loaded `load-screen.scr` preserved.
- New `STATE_SPLASH` case in dispatch loop; splash as `init/update/render` scene.
- `in_inkey()` lowercase (sdcc_iy clib, `<input/input_zx.h>`) for keypress detection.
- Static initializer in `game_state.c` (`GameState game_state = STATE_SPLASH;`); `game_state_init()` REMOVED.

## Build Evidence

Build command via Dev Containers CLI produces both artifacts (host-side confirmed):
- `bin/fixed-shoot-em-up.tap` (14047 bytes) — includes loaded `load-screen.scr` per Makefile `--screen` flag.
- `bin/fixed-shoot-em-up.sna` (49179 bytes) — 48K snapshot for emulators.

## Verifier Caveats

Two emulator scenarios remain DEFERRED to the user (per `openspec/config.yaml` no-test-runner policy):
- Fresh-key transitions splash → menu (code path verified statically; runtime needs Fuse).
- Held-key at boot does not skip splash (sentinel logic verified; runtime needs Fuse).

## Status

**SDD CYCLE COMPLETE.** The change is archived and the source of truth (canonical specs) is updated. Ready for the next change.
