# Design: Splash Screen

## Technical Approach

`main.c` flow: `im2_setup()` → `sp1_Initialize(MAKE_ROTTBL)` (NO `OVERWRITE_DFILE`/`OVERWRITE_TILES`, preserving loaded `load-screen.scr` pixels at `0x4000`) → `splash_scene_init()` → central dispatch loop (50 Hz `frame_tick` poll + `intrinsic_halt` + `switch (game_state)` + single `sp1_UpdateNow()`). Splash adopts the `init/update/render` triad like `level1`/`score`: `init` registers `font_1` tiles and prints the prompt via `sp1_PrintAt(.., FLASH)`; `update` does per-frame `in_inkey()` rising-edge keypoll; `render` is a no-op. `main.c` has NO call to `game_state_init()` (static initializer in `game_state.c` is the sole cold-boot mechanism) and NO `sp1_Invalidate(&game_area)` (would mark every cell dirty and clobber loaded art). Maps to `boot-splash` + `game-state-machine`.

## Architecture Decisions

### Decision: Splash font source = existing `font_1`

| Option | Tradeoff | Decision |
|---|---|---|
| Hand-encode 15×8 = 120 bytes in `splash.c` | Wasteful duplicate of data already in ROM as `font_1` | Rejected |
| Wire `font_full.png` pipeline | Overkill for one prompt line | Rejected |
| `sp1_TileEntry(c, font_1 + (c-32)*8)` for ASCII 32..127 | Zero new ROM data; matches `score.c` pattern; smaller `splash.c` | **Chosen** |

**Rationale**: `font_1` (768 bytes, ASCII 32-127) already ships in ROM via `font1.asm`; `score.c` proves the `(uint8_t *)font_1 + (c * 8)` indexing idiom.

### Decision: Splash lifecycle = standard `init/update/render` triad

| Option | Tradeoff | Decision |
|---|---|---|
| Blocking `splash_scene_run` pre-SP1 | Diverges from project pattern; can't dispatch cleanly in central loop | Rejected |
| `splash_scene_init` (once) + `splash_scene_update` (per frame) + `splash_scene_render` (no-op) | Matches `level1`/`score`; `main.c` central loop owns frame timing + `sp1_UpdateNow()` | **Chosen** |

**Rationale**: Splash runs after `sp1_Initialize`, so it can use SP1 like any scene; the central loop already owns the 50 Hz tick and single flush site.

### Decision: Keypress trigger = `in_inkey()` (lowercase, sdcc_iy)

| Option | Tradeoff | Decision |
|---|---|---|
| `in_KeyPressed(0x7F)` | Column mask 0x00 matches no key | Rejected |
| `in_Inkey()` (capital, generic `<input.h>`) | Does NOT link under `sdcc_iy` | Rejected |
| `in_inkey()` (lowercase, `<input/input_zx.h>`) rising-edge vs `prev = 0xFFu` | Only working "any key" detector the linker exports | **Chosen** |

**Rationale**: `in_inkey() ? 1u : 0u` with `prev = 0xFFu` sentinel rejects a held-at-boot key on the first poll and fires only on a fresh rising edge.

## Data Flow

```
[.tap bootstrap] ──→ 0x4000 (load-screen.scr)
       │
main()
  ├─ im2_setup()
  ├─ sp1_Initialize(MAKE_ROTTBL)        ← preserves 0x4000
  ├─ splash_scene_init():
  │     ├─ sp1_TileEntry(c, font_1 + (c-32)*8)
  │     ├─ sp1_PrintAt(20, col+i, INK|PAPER|FLASH, prompt[i])
  │     └─ prev = 0xFFu
  └─ dispatch loop (50 Hz):
        wait frame_tick; switch (game_state) {
            STATE_SPLASH:    splash_scene_update();   /* keypoll */
            STATE_PLAYING:   level1_inited one-shot + level1_update/render
            STATE_MENU:      halt placeholder
            STATE_GAME_OVER: halt placeholder
        }
        sp1_UpdateNow()                 ← single site; FLASH via ULA
```

## File Changes

| File | Action | Description |
|------|--------|-------------|
| `src/scenes/splash.h` | Modify | Replace `splash_scene_run` prototype with `splash_scene_update` + `splash_scene_render` (init already declared). |
| `src/scenes/splash.c` | Modify | 119 → ~40 lines. Drop `PROMPT_GLYPHS` table, pixel/attr writers, interleaved-bitmap math. Add `sp1_TileEntry` + `sp1_PrintAt` loops in init; `in_inkey()` rising-edge in update; render is a no-op. |
| `src/main.c` | Modify | Drop pre-SP1 splash block + `zx_cls` + `sp1_Invalidate`; drop `OVERWRITE_*` flags. Add `STATE_SPLASH` case calling `splash_scene_update()` + `splash_scene_render()`. Comment that `sp1_Initialize` omits `OVERWRITE_*` to preserve loaded art. |
| `src/game_state.{h,c}` | Resolved (already done) | Static initializer present; no `game_state_init` decl/definition. No change. |
| `Readme.md` | Already updated by orchestrator | Dev Containers CLI host-build section. |

## Interfaces / Contracts

```c
/* src/scenes/splash.h */
void splash_scene_init(void);    /* one-shot: register font_1, print prompt */
void splash_scene_update(void);   /* per-frame: in_inkey() rising-edge -> STATE_MENU */
void splash_scene_render(void);   /* no-op; sp1_UpdateNow() lives in main.c */
```

**Centering**: `PROMPT_LEN = 15`, `PROMPT_COL = (32 - 15) / 2 = 8`. Attribute byte `INK_WHITE | PAPER_BLACK | 0x80u` (FLASH). Writes via SP1 tile flush — no hand pixel/attr math, no raw `0x4000`/`0x5800` writes.

## Testing Strategy

| Layer | What | Approach |
|-------|------|----------|
| Build | Compile + link | `devcontainer exec --workspace-folder <repo> make ...` (canonical flags: `COMPILER=sdcc CLIB=sdcc_iy CRT=31 C_OPT_FLAGS='-SO3 --opt-code-size' CREATE_SNA=true CREATE_TAP=true`). |
| Static | Matches spec | Grep guards (verify): `OVERWRITE_*` absent in `main.c`; `game_state_init` zero in `src`; no raw `0x4000`/`0x5800` in `splash.c`; single `sp1_UpdateNow()` in `main.c`. |
| Manual | Art preserved; prompt blinks; fresh key transitions; held-at-boot doesn't skip | Fuse/emulator per `boot-splash` scenarios 4.2-4.5 (verify phase). |

## Migration / Rollout

No migration. Rollback per proposal: revert `main.c` and `splash.{h,c}` to the rejected pre-SP1 form (raw pixel/attr writes + blocking `_run` + `OVERWRITE_*` + glyph table). No asset/Makefile changes.

## Open Questions

- [ ] Confirm `in_inkey()` (lowercase) links under `sdcc_iy` — verified by build.
- [ ] Confirm SP1 budget tolerates ~95 ASCII `TileEntry` slots — verified by build; if not, register only the 14 prompt glyphs.

(Both resolve to PASS in verify if the build succeeds.)