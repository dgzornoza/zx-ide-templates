# boot-splash Specification

## Purpose

First-boot splash for `templates/fixed-shoot-em-up`: preserve the
`load-screen.scr` pixels loaded by the `.tap` bootstrap, show a blinking
`PULSA UNA TECLA` prompt on row 20 via SP1 + `font_1` tiles, and transition to
`STATE_MENU` on any debounced keypress. Splash runs once at cold boot, AFTER
`sp1_Initialize` (without `OVERWRITE_DFILE`/`OVERWRITE_TILES`), and never
re-enters via transitions.

## Requirements

### Requirement: Boot Preserves Loaded Screen Pixels

The system SHALL NOT wipe the `load-screen.scr` pixels loaded by the `.tap`
bootstrap while `game_state == STATE_SPLASH`. `sp1_Initialize` SHALL be called
WITHOUT `SP1_IFLAG_OVERWRITE_DFILE` or `SP1_IFLAG_OVERWRITE_TILES`, so the
loaded pixels survive SP1 init and remain visible for the splash duration. No
`zx_cls` or full-screen pixel wipe SHALL run during splash.

#### Scenario: Pixels intact during splash

- GIVEN the `.tap` build has loaded `load-screen.scr` into screen memory
- WHEN `main.c` reaches the splash phase after `sp1_Initialize`
- THEN the loaded pixels remain visible for the entire splash duration
- AND row 20 cells are SP1-managed (prompt), while the rest stays as loaded
- AND no `zx_cls` or pixel wipe runs while `game_state == STATE_SPLASH`

#### Scenario: Loaded art not clobbered if OVERWRITE_DFILE absent

- GIVEN `main.c` calls `sp1_Initialize` to bring SP1 up
- WHEN the `sp1_Initialize` call is inspected
- THEN the flag mask does NOT include `SP1_IFLAG_OVERWRITE_DFILE`
- AND the flag mask does NOT include `SP1_IFLAG_OVERWRITE_TILES`
- AND the loaded pixel art is preserved into the splash

### Requirement: Prompt Rendered via font_1 Tiles on Row 20

The system SHALL show `PULSA UNA TECLA` horizontally centered on display row
20 by registering the `font_1` ASCII tiles via `sp1_TileEntry` (tile IDs
ASCII-aligned per `TILE_FONT_BASE = ' '`) and calling
`sp1_PrintAt(20, (32 - 15) / 2 + i, INK_WHITE | PAPER_BLACK | 0x80u, prompt[i])`
for each char. `splash.c` SHALL NOT hand-encode glyph bitmaps; the glyph data
SHALL come from `font_1` at `src/data/fonts/font1.asm`. Blinking SHALL be driven
by the hardware FLASH attribute bit (`0x80u`); zero CPU per frame.

#### Scenario: Prompt shown centered with FLASH

- GIVEN `splash_scene_init()` has registered the `font_1` tiles and printed the prompt
- WHEN the central loop calls `sp1_UpdateNow()`
- THEN the `PULSA UNA TECLA` cells appear horizontally centered on row 20
- AND each prompt cell's attribute byte has the FLASH bit (`0x80u`) set
- AND visibility toggles via the ULA, with no per-frame CPU toggling in splash

#### Scenario: font_1 tiles used (no hand-encoded glyph table)

- GIVEN `splash.c` is implementing the prompt rendering
- WHEN the source is inspected
- THEN `splash.c` references the `font_1` tile data via `sp1_TileEntry`
- AND `splash.c` does NOT define a `static const ... GLYPHS[...]` (or equivalent) glyph-table array

#### Scenario: SP1 renders the prompt (not direct RAM writes)

- GIVEN the prompt is being drawn on row 20
- WHEN `splash.c` is inspected
- THEN the prompt is issued through `sp1_PrintAt`
- AND `splash.c` does NOT write directly to `0x4000` (pixel file) or `0x5800` (attribute file)

### Requirement: Any-Keypress Trigger With Debounce (in_inkey)

The system SHALL detect the trigger keypress via rising-edge detection on
`in_inkey()`. `in_inkey()` is the LOWERCASE symbol exported by the `sdcc_iy`
clib from `<input/input_zx.h>`; the capital-I `in_Inkey()` symbol from generic
`<input.h>` does NOT link under `sdcc_iy` and MUST NOT be used. `splash.c`
MUST include `<input/input_zx.h>`. A key held at boot SHALL NOT skip the splash;
the transition SHALL fire only when key state differs across consecutive
`frame_tick` samples (rising edge). The initial `prev` sample SHALL be `0xFFu`
(released sentinel) so a held key cannot trigger on the first poll.

#### Scenario: Fresh keypress transitions to STATE_MENU

- GIVEN no key is held at splash entry and `prev` sampled a released state
- WHEN a key is pressed and observed on the current `frame_tick`
- THEN `game_state` transitions from `STATE_SPLASH` to `STATE_MENU`
- AND the splash returns control to `main.c`'s central loop

#### Scenario: Held key at boot does not skip

- GIVEN a key is already held when the splash starts and `prev = 0xFFu`
- WHEN the splash samples key state on consecutive `frame_tick` values
- THEN the held key produces identical consecutive samples
- AND splash does NOT transition until the key is released and re-pressed

### Requirement: First-Boot Only

`STATE_SPLASH` SHALL be the cold-boot value of `game_state`, set by the static
initializer `GameState game_state = STATE_SPLASH;` in `src/game_state.c`. No
scene transition (game-over to menu, menu restart, future flow) SHALL set
`game_state = STATE_SPLASH`. The function `game_state_init()` SHALL NOT exist;
cold-boot state is provided solely by the static initializer.

#### Scenario: Never re-entered via transition

- GIVEN the run has already left `STATE_SPLASH` once
- WHEN any subsequent scene transition executes
- THEN `game_state` is NOT assigned `STATE_SPLASH`
- AND `grep -n "STATE_SPLASH = ==" src` shows only the `game_state.c` static initializer line

#### Scenario: game_state_init does not exist

- GIVEN the codebase implements the splash contract
- WHEN `grep -rn "game_state_init" src` is run
- THEN the search returns zero matches
- AND the splash is entered purely via the static initializer

### Requirement: Splash Does No Global Init

`splash_scene_init/update/render` SHALL NOT call `zx_cls`, `zx_border`,
`sp1_Initialize`, or `sp1_Invalidate`. `sp1_UpdateNow()` is owned by `main.c`'s
central loop and SHALL NOT be called from `splash_scene_render` (which is a
no-op).

#### Scenario: No zx_/sp1_ init calls in splash.c

- GIVEN `splash.c` implements the scene entry points
- WHEN `splash.c` is inspected
- THEN it contains no calls to `zx_cls`, `zx_border`, `sp1_Initialize`, or `sp1_Invalidate`

#### Scenario: Single UpdateNow site in main.c

- GIVEN the central loop in `main.c` drives rendering
- WHEN the codebase is searched for `sp1_UpdateNow()`
- THEN there is exactly one call site, located in `main.c`'s central loop
- AND `splash_scene_render` does not call `sp1_UpdateNow()`