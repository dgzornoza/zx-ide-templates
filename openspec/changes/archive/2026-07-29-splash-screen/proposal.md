# Proposal: Splash Screen

## Intent

Rework splash as an SP1-native scene that reuses `font_1`, conforms to `init/update/render`, and preserves the loaded `load-screen.scr` by leaving SP1 init off `OVERWRITE_DFILE`/`OVERWRITE_TILES`. The pre-SP1 hand-encoded-glyphs approach is dropped.

## Scope

### In Scope
- Rewrite `splash.h`/`.c` to `init/update/render` using `font_1` (`sp1_TileEntry` + `sp1_PrintAt` with FLASH on row 20).
- Restructure `main.c`: drop pre-SP1 splash + `game_state_init` call; init SP1 without `OVERWRITE_*`; `switch (game_state)` with `STATE_SPLASH` case.
- Rising-edge debounce via `in_inkey()` (lowercase — sdcc_iy clib) with `prev = 0xFFu` sentinel.

### Out of Scope
- Main menu / game over (stubs remain).
- Reintroducing `game_state_init` — state is statically initialized.
- Title artwork, copyright, auto-timeout.

## Capabilities

### Modified Capabilities
- `boot-splash`: render switches from raw RAM writes to `sp1_TileEntry(font_1)` + `sp1_PrintAt(.., FLASH)`. Splash runs AFTER `sp1_Initialize` (no `OVERWRITE_DFILE`), not before.
- `game-state-machine`: cold-boot initial state is the static initializer `GameState game_state = STATE_SPLASH;`. `game_state_init` REMOVED; `main.c` inits SP1 once and dispatches `STATE_SPLASH` like any other state.

## Approach

1. `sp1_Initialize(MAKE_ROTTBL)` — loaded pixels preserved.
2. `splash_scene_init()`: register `font_1` ASCII tiles via `sp1_TileEntry`; print prompt via `sp1_PrintAt(20, col+i, INK|PAPER|FLASH, prompt[i])`; `prev = 0xFFu`.
3. `splash_scene_update()`: rising-edge on `in_inkey()` sets `game_state = STATE_MENU`.
4. `splash_scene_render()`: no-op; `sp1_UpdateNow()` stays in `main.c`.
5. Dispatch: `STATE_SPLASH` → splash_update; `STATE_PLAYING` → `level1_inited` one-shot; menu/game-over halt.

## Affected Areas

| File | Change |
|------|--------|
| `src/scenes/splash.h` | Drop `_run`; expose `init/update/render`. |
| `src/scenes/splash.c` | 119→~40 lines; tile registry + sp1 prints; no raw writes. |
| `src/main.c` | Drop pre-SP1 splash + `game_state_init`; drop `OVERWRITE_*`; add `STATE_SPLASH` case. |
| `src/game_state.{h,c}` | Resolved (static initializer; function removed). |
| `Readme.md` | New lifecycle. |

## Risks

| Risk | Mitigation |
|------|------------|
| Splash re-appears on later transitions | `STATE_SPLASH` set only by static initializer. |
| Loaded art wiped if `OVERWRITE_DFILE` re-added | Document flag contract in `main.c`; grep guard in criteria. |
| ~95 ASCII `TileEntry` slots exceed SP1 budget | If tight, register only the prompt glyphs. |

## Rollback Plan

Revert `main.c` to the rejected pre-SP1 form (raw pixel/attr writes, blocking `_run`, `OVERWRITE_*`). Restore `splash.c` glyph table. No Makefile/asset changes.

## Dependencies

`load-screen.scr` via Makefile `CREATE_TAP`; z88dk `in_inkey()` (lowercase); `font_1` + `TILE_FONT_BASE`; IM2 ISR `frame_tick`.

## Success Criteria

- [ ] `bin/*.tap` + `bin/*.sna` build via `devcontainer exec ... make`.
- [ ] Boot: loaded pixels preserved; `PULSA UNA TECLA` blinks row 20 via FLASH.
- [ ] Fresh key → `STATE_SPLASH` → `STATE_MENU`; held-at-boot does not skip.
- [ ] Splash never re-appears on later transitions.
- [ ] `splash.c` ~40 lines, uses `font_1`, no glyph table.
- [ ] `grep sp1_Initialize main.c` shows no `OVERWRITE_DFILE`; `grep game_state_init src` returns zero.