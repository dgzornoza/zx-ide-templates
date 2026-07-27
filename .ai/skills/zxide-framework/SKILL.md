---
name: zxide-framework
description: "Use when adding, modifying, or auditing framework-level configuration in the ZX-IDE framework — compile-time flags, game-level constants, player identifiers, or any value that lives in the `core/*_definitions.h` family of headers. Trigger when working on framework customization, template-level RAM/ROM budgets, or any value that affects the public API surface."
user-invocable: true
---

# ZX-IDE Framework Configuration

The ZX-IDE framework's compile-time configuration is **segregated by category** under `templates/fixed-shoot-em-up/src/core/`. Each category gets its own header so per-template, per-game, and per-feature customization happens in one well-defined place — never inline in consumer modules.

## 1. The Definitions Header Family

The current layout (open for further segregation by category):

| Header | Category | Examples |
|---|---|---|
| `core/system_definitions.h` | System (hardware constants) | `SCREEN_WIDTH`, `SCREEN_HEIGHT`, `CHAR_SIZE` |
| `core/game_definitions.h`   | Game (game-level config)   | `ALLOWED_GAME_PLAYERS`, `PlayerId` |
| _(future) `core/framework_definitions.h`_ | Framework opt-in flags | `FRAMEWORK_USE_<MODULE>` |

The deciding question for where a new constant goes: **"Is this a property of the machine, the game, or the framework being optional?"**

- **Machine** → `system_definitions.h`. Immutable on the target; never per-project.
- **Game** → `game_definitions.h`. This game/template chose this value; another game may pick differently.
- **Framework opt-in** → own file (e.g. `framework_definitions.h`) when the first flag is introduced. Not game-specific.

### 1.1 System defined constants (`system_definitions.h`)

Hard-coded ZX Spectrum hardware values. Examples: `SCREEN_WIDTH = 256`, `SCREEN_HEIGHT = 192`, `CHAR_SIZE = 8`. Do NOT push platform-specific values below this line.

### 1.2 Game-level constants (`game_definitions.h`)

Values that the game/template picks and that affect memory layout and the exposed API surface. Example: `ALLOWED_GAME_PLAYERS` (number of simultaneous players). Pattern:

```c
#ifndef MY_CONSTANT
#define MY_CONSTANT <default>
#endif

#if (MY_CONSTANT < MIN) || (MY_CONSTANT > MAX)
#error "MY_CONSTANT must be in [MIN..MAX]"
#endif
```

Always validate with `#if/#error` so misconfiguration fails at compile time.

The naming convention is `<DOMAIN>_<ASPECT>` (e.g. `ALLOWED_GAME_PLAYERS`, not `INPUT_MAX_PLAYERS`): the constant's name must point at the level that OWNS the decision, not at the level that merely consumes it. The input manager sizes its player array from `ALLOWED_GAME_PLAYERS`, but the input module does not own the value — the game does.

### 1.3 Player identifiers (in `game_definitions.h`, sized to a game-level constant)

When the framework exposes an enum sized by a game-level constant, define it conditionally:

```c
#if ALLOWED_GAME_PLAYERS >= 2
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
```

This makes references to non-existent slots fail at compile time. The first entry is always index 0; additional entries are gated by the game-level constant.

### 1.4 Framework compile-time flags (future `framework_definitions.h`)

Opt-in/out switches for optional framework modules. Currently none are defined; when the first one is introduced, it goes in its own header (NOT in `game_definitions.h` — framework flags are not game-level). Pattern:

```c
#ifndef FRAMEWORK_USE_<NAME>
#define FRAMEWORK_USE_<NAME> <default 0 or 1>
#endif
```

Add a flag here when a framework module becomes optional. Document the trade-off (RAM/ROM cost, when to enable) in the comment above the `#ifndef`. Override per-project via the Makefile (`-DFRAMEWORK_USE_<NAME>=0`) or by editing the default below.

Do NOT add flags speculatively for modules that don't exist yet — they add noise without value.

## 2. Adding a New Framework Flag (when the time comes)

When a new framework module becomes optional:

1. Implement the module with the API surface you want.
2. Create `framework_definitions.h` if it does not exist yet, under the `Framework compile-time flags` section:
   ```c
   #ifndef FRAMEWORK_USE_<MODULE>
   #define FRAMEWORK_USE_<MODULE> 1   // default ON for first release
   #endif
   ```
3. Wrap the module's include or implementation in the consumer(s):
   ```c
   #if FRAMEWORK_USE_<MODULE>
   // module usage
   #endif
   ```
4. Document the trade-off in the comment above the `#ifndef`:
   - Default value (0 = OFF, 1 = ON)
   - What gets compiled in vs out
   - Approximate RAM/ROM cost when enabled
   - When to enable / disable

## 3. Boundary Rules — What Does NOT Belong in Any `*_definitions.h`

These stay in the consumer module. Do NOT migrate them:

| Constant type | Where it belongs | Example |
|---|---|---|
| Per-asset dimensions | Consumer's `data/` header | `HUD_MAP_WIDTH = 16` in `hud-map.h` |
| Per-scene UI layout | Scene `.c` file | `SCORE_X_POS = 21` in `score.c` |
| Per-gameplay tuning | Entity feature `.c` | `PLAYER_INITIAL_X = 120` in `player.c` |
| Hard-coded hardware values | `system_definitions.h` | `SCREEN_WIDTH = 256` (already there) |
| Game-level config | `game_definitions.h` | `ALLOWED_GAME_PLAYERS` |

The deciding question: **"Could a different game/template reasonably want a different value?"**

- **YES** → `game_definitions.h` (or another category header).
- **NO** → consumer module (game-specific or implementation-specific).

When in doubt, leave it in the consumer. Moving a constant to a definitions header is cheap; migrating it back after a misuse is messy.

## 4. Worked Example: `ALLOWED_GAME_PLAYERS`

The current state of the framework (post-renaming from `INPUT_MAX_PLAYERS`):

```c
// game_definitions.h — Game-level constants
#ifndef ALLOWED_GAME_PLAYERS
#define ALLOWED_GAME_PLAYERS 1
#endif

#if (ALLOWED_GAME_PLAYERS < 1) || (ALLOWED_GAME_PLAYERS > 2)
#error "ALLOWED_GAME_PLAYERS must be 1 or 2 currently"
#endif

// game_definitions.h — Player identifiers
#if ALLOWED_GAME_PLAYERS >= 2
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
```

Consumers (`input_manager.c`, `level1.c`, etc.) gate code that touches the second player:

```c
#if ALLOWED_GAME_PLAYERS >= 2
    input_reset_defaults(PLAYER_2);
#endif
```

Measured savings when compiling with `-DALLOWED_GAME_PLAYERS=1`:

- 13 B RAM (one less `InputPlayerState` in BSS)
- ~83 B ROM (no second-player poll path)

## 5. Cross-references

- `convention/framework-config-definitions-h` (Engram observation) — cross-session memory.
- `.ai/rules/project-architecture.md` — source-tree placement (separate concern; this skill is about WHAT goes in the definitions headers, that rule is about WHERE source files live).
- `.ai/skills/z88dk-c-coding-conventions/SKILL.md` — C style and 8-bit optimization patterns that apply when writing the framework code itself.