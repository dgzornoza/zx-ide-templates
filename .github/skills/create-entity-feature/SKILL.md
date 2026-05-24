---
name: create-entity-feature
description: "Use when creating gameplay entity features (player, enemies, bullets, pickups) under scenes/features/entities."
user-invocable: true
argument-hint: "Describe entity purpose, pool size (if multi-instance), and owning gameplay scene."
---

# Create Gameplay Entity Feature Workflow

## When to Use

Use this skill when implementing a gameplay entity feature that belongs to scene orchestration.

- Examples: `player`, `enemy1`, `enemy2`, `bullet`, `pickup`.
- Target folder: `src/scenes/features/entities/`.

## Architectural Constraints (CRITICAL)

Follow architecture and platform constraints from instructions:

- `.github/instructions/project-architecture.instructions.md` (scene/feature ownership)
- `.github/instructions/z80-c-architecture.instructions.md` (CPU/memory constraints)
- `.github/instructions/sp1-rendering.instructions.md` (SP1/frame loop constraints)

## Procedure

1. Create `<entity>.h` in `src/scenes/features/entities/` and expose only callbacks used by the owning scene:

- `<entity>_init(void)`
- `<entity>_update(void)`
- `<entity>_render(void)`
- `<entity>_reset(void)` only when multi-instance or level reset requires it

2. Create `<entity>.c` in `src/scenes/features/entities/` with file-local `static` state.

3. If 2 or more simultaneous instances are required:

- Add fixed-size pool.
- Define pool size macro in header (for example `#define ENEMY1_POOL_SIZE 4`).

4. Read shared run-level data from `game_state.h` when needed; do not pass large state objects through APIs.

5. Integrate callback calls in the owning gameplay scene under `src/scenes/gameplay/` following required update order.

6. Keep rendering differential; do not call `sp1_UpdateNow()` from the entity module.

## Output

When complete, provide:

1. Entity files created/updated under `src/scenes/features/entities/`.
2. Lifecycle callbacks exposed and which are intentionally omitted.
3. Pool strategy and reset policy.
4. Owning scene integration points.
