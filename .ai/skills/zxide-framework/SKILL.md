---
name: zxide-framework
description: "Use when adding, modifying, or auditing framework-level configuration in the ZX-IDE framework — compile-time flags, per-template constants, player identifiers, or any value that lives in `definitions.h`. Trigger when working on framework customization, template-level RAM/ROM budgets, or any value that affects the public API surface."
user-invocable: true
---

# ZX-IDE Framework Configuration

The ZX-IDE framework's compile-time configuration lives in **one place**: `templates/fixed-shoot-em-up/src/core/definitions.h`. Per-game, per-template, and per-feature customization happens there — never inline in consumer modules.

## 1. The Three Sections of `definitions.h`

In order:

### 1.1 System defined constants

Hard-coded ZX Spectrum hardware values. Examples: `SCREEN_WIDTH = 256`, `SCREEN_HEIGHT = 192`, `CHAR_SIZE = 8`. Do NOT push platform-specific values below this line.

### 1.2 Framework compile-time flags

Opt-in/out switches for optional framework modules. Pattern:

```c
#ifndef FRAMEWORK_USE_<NAME>
#define FRAMEWORK_USE_<NAME> <default 0 or 1>
#endif
```

Add a flag here when a framework module becomes optional. Document the trade-off (RAM/ROM cost, when to enable) in the comment above the `#ifndef`. Override per-project via the Makefile (`-DFRAMEWORK_USE_<NAME>=0`) or by editing the default below.

Do NOT add flags speculatively for modules that don't exist yet — they add noise without value.

### 1.3 Per-template constants

Values like `INPUT_MAX_PLAYERS` that affect memory layout and the exposed API surface. Pattern:

```c
#ifndef MY_CONSTANT
#define MY_CONSTANT <default>
#endif

#if (MY_CONSTANT < MIN) || (MY_CONSTANT > MAX)
#error "MY_CONSTANT must be in [MIN..MAX]"
#endif
```

Always validate with `#if/#error` so misconfiguration fails at compile time.

### 1.4 Player identifiers (sized to a per-template constant)

When the framework exposes an enum sized by a per-template constant, define it conditionally:

```c
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
```

This makes references to non-existent slots fail at compile time. The first entry is always index 0; additional entries are gated by the per-template constant.

## 2. Adding a New Framework Flag

When a new framework module becomes optional:

1. Implement the module with the API surface you want.
2. Add the flag to `definitions.h` under "Framework compile-time flags":
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

## 3. Boundary Rules — What Does NOT Belong in `definitions.h`

These stay in the consumer module. Do NOT migrate them:

| Constant type | Where it belongs | Example |
|---|---|---|
| Per-asset dimensions | Consumer's `data/` header | `HUD_MAP_WIDTH = 16` in `hud-map.h` |
| Per-scene UI layout | Scene `.c` file | `SCORE_X_POS = 21` in `score.c` |
| Per-gameplay tuning | Entity feature `.c` | `PLAYER_INITIAL_X = 120` in `player.c` |
| Hard-coded hardware values | System section above | `SCREEN_WIDTH = 256` (already there) |

The deciding question: **"Could a different game/template reasonably want a different value?"**

- **YES** → `definitions.h` (framework-level).
- **NO** → consumer module (game-specific or implementation-specific).

When in doubt, leave it in the consumer. Moving a constant to `definitions.h` is cheap; migrating it back after a misuse is messy.

## 4. Worked Example: `INPUT_MAX_PLAYERS`

The current state of the framework (as of the input-manager-multiplayer change):

```c
// definitions.h — Per-template constants
#ifndef INPUT_MAX_PLAYERS
#define INPUT_MAX_PLAYERS 2
#endif

#if (INPUT_MAX_PLAYERS < 1) || (INPUT_MAX_PLAYERS > 2)
#error "INPUT_MAX_PLAYERS must be 1 or 2 on the current 48K target"
#endif

// definitions.h — Player identifiers
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
```

Consumers (`input_manager.c`, `level1.c`, etc.) gate code that touches the second player:

```c
#if INPUT_MAX_PLAYERS >= 2
    input_reset_defaults(PLAYER_2);
#endif
```

Measured savings when compiling with `-DINPUT_MAX_PLAYERS=1`:

- 13 B RAM (one less `InputPlayerState` in BSS)
- ~83 B ROM (no second-player poll path)

## 5. Cross-references

- `convention/framework-config-definitions-h` (Engram observation) — cross-session memory.
- `.ai/rules/project-architecture.md` — source-tree placement (separate concern; this skill is about WHAT goes in `definitions.h`, that rule is about WHERE source files live).
- `.ai/skills/z88dk-c-coding-conventions/SKILL.md` — C style and 8-bit optimization patterns that apply when writing the framework code itself.