# Tasks: Splash Screen

## Review Workload Forecast

| Field | Value |
|-------|-------|
| Estimated changed lines | ~80–100 (splash.c 119→~40; main.c −10/+15; splash.h −3) |
| 400-line budget risk | Low |
| Chained PRs recommended | No |
| Suggested split | single PR |
| Delivery strategy | ask-on-risk |
| Chain strategy | pending |

Decision needed before apply: No
Chained PRs recommended: No
Chain strategy: pending
400-line budget risk: Low

### Suggested Work Units

Not needed. Single PR; diff well under 400-line budget.

## Phase 1: Foundation (verify prior edits intact)

- [x] 1.1 Verify `templates/fixed-shoot-em-up/src/game_state.h` declares `GameState` with `STATE_SPLASH` first and NO `game_state_init` prototype.
- [x] 1.2 Verify `templates/fixed-shoot-em-up/src/game_state.c` has `GameState game_state = STATE_SPLASH;` as the static initializer and NO `game_state_init` definition.

## Phase 2: Splash scene rewrite

- [x] 2.1 Rewrite `templates/fixed-shoot-em-up/src/scenes/splash.h` to expose `splash_scene_init`, `_update`, `_render` (drop `_run`); keep include guards.
- [x] 2.2 Rewrite `templates/fixed-shoot-em-up/src/scenes/splash.c` (~40 lines): include `<input/input_zx.h>`, sp1 via `z88dk_headers.h`, `tile_registry.h`, `font_1.h`, `game_state.h`; add `static uint8_t prev_key = 0xFFu;` file-local.
- [x] 2.3 In `splash_scene_init()`: register ASCII 32..127 via `sp1_TileEntry(c, (uint8_t *)font_1 + ((c - 32u) * 8u))`; print `"PULSA UNA TECLA"` via `sp1_PrintAt(20u, 8u + i, INK_WHITE | PAPER_BLACK | 0x80u, prompt[i])`.
- [x] 2.4 In `splash_scene_update()`: `cur = in_inkey() ? 1u : 0u;` on `prev_key == 0u && cur != 0u` set `game_state = STATE_MENU`; `prev_key = cur`.
- [x] 2.5 `splash_scene_render()`: empty body (no-op; `sp1_UpdateNow()` lives in `main.c`).

## Phase 3: main.c restructure

- [x] 3.1 In `templates/fixed-shoot-em-up/src/main.c`: remove the pre-SP1 `if (game_state == STATE_SPLASH) { splash_scene_init(); splash_scene_run(); }` block.
- [x] 3.2 Drop `SP1_IFLAG_OVERWRITE_TILES` and `SP1_IFLAG_OVERWRITE_DFILE` from `sp1_Initialize(...)` flags; keep only `SP1_IFLAG_MAKE_ROTTBL`; add a comment that `OVERWRITE_*` is intentionally absent so loaded `load-screen.scr` survives.
- [x] 3.3 Remove `zx_border(PAPER_BLACK)`, `zx_cls(PAPER_BLACK)`, and `sp1_Invalidate(&game_area)` calls (clobber loaded art).
- [x] 3.4 Confirm `splash_scene_init()` is called ONCE after `sp1_Initialize`, before the dispatch loop; `#include "scenes/splash.h"` stays in place.
- [x] 3.5 Confirm `static uint8_t level1_inited = 0u;` file-scope flag still guards `level1_scene_init()` as one-shot on first `STATE_PLAYING` entry.
- [x] 3.6 In `switch (game_state)`, add `case STATE_SPLASH:` calling `splash_scene_update();` + `splash_scene_render();`; keep `STATE_PLAYING` / `STATE_MENU` / `STATE_GAME_OVER` branches unchanged.

## Phase 4: Verification

- [x] 4.1 Build via Dev Containers CLI on host: `devcontainer exec --workspace-folder "<repo-root>" bash -c "cd /workspaces/zx-ide-templates/templates/fixed-shoot-em-up && make COMPILER=sdcc CLIB=sdcc_iy CRT=31 'C_OPT_FLAGS=-SO3 --opt-code-size' LINKER_FLAGS= CREATE_SNA=true CREATE_TAP=true"` — both `.tap` and `.sna` must produce. PASS = link succeeds on lowercase `in_inkey`.
- [x] 4.2 Static grep guards (host, no build): `OVERWRITE_DFILE\|OVERWRITE_TILES` in `main.c` → 0 matches; `game_state_init` in `src/` → 0 matches; `sp1_UpdateNow` in `main.c` → 1 match; `sp1_UpdateNow` in `splash.c` → 0; `0x4000\|0x5800\|GLYPHS` in `splash.c` → 0 (no raw writes, no glyph table).
- [x] 4.3 `wc -l templates/fixed-shoot-em-up/src/scenes/splash.c` ≤ 50 (down from 119).
- [x] 4.4 Fuse/emulator per `boot-splash` scenarios: loaded `load-screen.scr` art preserved; `PULSA UNA TECLA` blinks row 20 via FLASH; held-at-boot does not skip; fresh press transitions to `STATE_MENU`.

## Phase 5: Cleanup

- [x] 5.1 Update `templates/fixed-shoot-em-up/Readme.md` "Features" section to one concise English sentence describing the SP1-tile-based splash.