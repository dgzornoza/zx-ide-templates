---
name: create-gameplay-scene
description: "Use when creating or refactoring playable gameplay scenes under src/scenes/gameplay with explicit orchestration order."
user-invocable: true
argument-hint: "Describe gameplay goal, entities/features involved, and expected transitions."
---

# Create Gameplay Scene Workflow

## When to Use

Use this skill for playable scenes that run gameplay loops.

- Target folder: `src/scenes/gameplay/`.
- Example files: `level1.c`, `boss_room.c`.

## Constraints

Follow:

- `.ai/rules/project-architecture.md`
- `.ai/rules/z80-c-architecture.md`
- `.ai/rules/sp1-rendering.md`

## Procedure

1. Create scene header and source in `src/scenes/gameplay/`.
2. Expose `<scene>_init(void)` and `<scene>_update(void)` by default. Add `<scene>_render(void)` ONLY when the scene owns a per-frame ordering decision that the orchestrator cannot make (e.g., layering groups whose SP1 column occlusion order matters). When in doubt, omit it.
3. When `_render` is omitted, the dirty-marker phase of feature `_render` calls runs at the end of `<scene>_update`, before transition checks.
4. Wire feature dependencies from:

- `src/scenes/features/entities/`
- `src/scenes/features/ui/`

4. Keep update order fixed inside `<scene>_update`:

- input
- player
- enemies
- collisions
- sound dispatch
- dirty-marker phase (entity `_render` + UI `_render` calls)
- transition checks

5. Call feature resets before scene transitions when required.
6. Keep render differential and never flush (`sp1_UpdateNow`) inside scene module.

## Output

When complete, report:

1. Scene files created/updated.
2. Feature callbacks orchestrated and update order.
3. Transition/reset behavior.
4. Integration points used by scene dispatcher.
