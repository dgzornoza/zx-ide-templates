# game-state-machine Specification

## Purpose

Define the `GameState` enum for `templates/fixed-shoot-em-up`, its cold-boot
initial value (a static initializer in `src/game_state.c`), the constraint that
`STATE_SPLASH` is only set at cold boot, and the central dispatch contract in
`main.c` that routes `game_state` to the corresponding scene.

## Requirements

### Requirement: GameState Enum Membership

The `GameState` enum SHALL include `STATE_SPLASH`, `STATE_MENU`,
`STATE_PLAYING`, and `STATE_GAME_OVER`. `STATE_SPLASH` SHALL be the first
value (the cold-boot entry state); the others are downstream scene states.

#### Scenario: Enum contains all states with splash first

- GIVEN `game_state.h` declares the `GameState` enum
- WHEN the enum is inspected
- THEN it contains `STATE_SPLASH`, `STATE_MENU`, `STATE_PLAYING`, and `STATE_GAME_OVER`
- AND `STATE_SPLASH` is the first/initial value used at cold boot

### Requirement: Cold-Boot Initial State (static initializer)

`src/game_state.c` SHALL declare `GameState game_state = STATE_SPLASH;` as a
static initializer — the sole mechanism that selects the splash as the entry
scene. The function `game_state_init()` SHALL NOT exist (it has been removed).
`main.c` SHALL NOT call `game_state_init()`.

#### Scenario: Static initializer sets splash at cold boot

- GIVEN the program is starting from a cold boot
- WHEN `src/game_state.c` is compiled
- THEN `game_state` is declared as `GameState game_state = STATE_SPLASH;`
- AND the splash is entered because that is the static initial value

#### Scenario: No game_state_init function or call

- GIVEN the codebase implements the cold-boot state contract
- WHEN `grep -rn "game_state_init" src` is run
- THEN the search returns zero matches
- AND `main.c` does not call any init function to set the initial state

#### Scenario: Init does not run splash after boot

- GIVEN the run has already progressed past the splash once
- WHEN the post-splash flow executes
- THEN state is set ONCE at static init time
- AND there is no runtime mechanism to re-enter `STATE_SPLASH` (no init function exists)

### Requirement: No Transition Sets STATE_SPLASH

No code path that runs after `main()`'s dispatch loop SHALL set
`game_state = STATE_SPLASH`. Transitions between scenes MUST target
`STATE_MENU`, `STATE_PLAYING`, or `STATE_GAME_OVER` — never `STATE_SPLASH`.

#### Scenario: Transitions avoid splash

- GIVEN a scene transition is being executed anywhere in the codebase
- WHEN the transition assigns a new value to `game_state`
- THEN the assigned value is one of `STATE_MENU`, `STATE_PLAYING`, or `STATE_GAME_OVER`
- AND it is NOT `STATE_SPLASH`
- AND `grep "STATE_SPLASH =" src/game_state.c` shows only the static initializer line

#### Scenario: Held assertion across future flows

- GIVEN the menu, gameplay, and game-over scenes are implemented
- WHEN each scene's update logic decides its next state
- THEN none of them writes `STATE_SPLASH`
- AND the splash cannot re-appear later in the run

### Requirement: Central Dispatcher Reads game_state

`main.c` SHALL run a `while(1) { ... switch(game_state) { ... } }` loop driven
by the 50Hz `frame_tick` poll and `intrinsic_halt`. `main.c` SHALL call
`sp1_Initialize` ONCE before the loop, then dispatch `STATE_SPLASH` like any
other state. `main.c` remains an orchestrator only and SHALL NOT own
scene-local state.

- `STATE_SPLASH` runs `splash_scene_update();` (and `splash_scene_render();` if non-empty).
- `STATE_PLAYING` runs `level1_scene_init()` once (guarded by a `level1_inited` flag) + `level1_scene_update()` + `level1_scene_render()`.
- `STATE_MENU` and `STATE_GAME_OVER` use `for(;;) intrinsic_halt();` placeholders.

#### Scenario: Dispatch routes by state

- GIVEN `game_state` holds a value after the static initializer or a transition
- WHEN `main.c` evaluates the dispatch loop
- THEN the scene matching the current `game_state` is invoked
- AND `STATE_PLAYING`'s one-shot init is gated by a `level1_inited` flag
- AND `main.c` does not embed scene-local logic or state

#### Scenario: Post-splash init precedes dispatch

- GIVEN `main.c` is bringing SP1 up before the dispatch loop
- WHEN `main.c` runs
- THEN `sp1_Initialize` is called ONCE before the loop
- AND the loop then dispatches `STATE_SPLASH` like any other state
- AND no later wipe or `sp1_Initialize` re-init runs between splash and menu