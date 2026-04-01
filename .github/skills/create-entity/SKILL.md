---
name: create-entity
description: "Use when the user asks to create a new game entity, character, or element (e.g., player, enemy, bullet). It enforces the zero-parameter and global-state architecture."
user-invocable: true
---

# Create Game Entity Workflow

## When to Use

Use this skill when implementing a new visual or logical component for the game (e.g., a player, an enemy, a bullet manager).

## Architectural Constraints (CRITICAL)

- **Zero dynamic memory**: Never use `malloc`/`free`. If multiple instances are needed (like bullets), use a fixed-size `static` array.
- **Zero-parameter functions**: Avoid passing arguments to `update` or `draw` methods to save Z80 stack cycles.

## Procedure

1. Create a `.h` header file exposing ONLY the public signatures (e.g., `bullet_init(void)`, `bullet_update(void)`, `bullet_draw(void)`).
2. Create a `.c` file and define the entity's properties (x, y, hp, is_active, etc.) as `static` global variables at the top of the file so they are restricted to that file namespace.
3. If the entity needs to be aware of the game state (score, frame ticks), `#include "../../state/game_state.h"` and read the states directly instead of passing them as arguments.
4. Implement the logic functions operating strictly on those `static` variables.
5. Create an `update` function that updates the state and compares values with `last_drawn_*` tracking variables, followed by redrawing with `sp1_UpdateNow()` calls.

## Output

When complete, provide a summary of the `.h` and `.c` files created and instruct the user on how to call the `_init()` method in `main.c`.
