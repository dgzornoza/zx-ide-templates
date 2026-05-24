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

- `.github/instructions/project-architecture.instructions.md`
- `.github/instructions/z80-c-architecture.instructions.md`
- `.github/instructions/sp1-rendering.instructions.md`

## Procedure

1. Create scene header and source in `src/scenes/gameplay/`.
2. Expose `<scene>_init(void)`, `<scene>_update(void)`, `<scene>_render(void)`.
3. Wire feature dependencies from:

- `src/scenes/features/entities/`
- `src/scenes/features/ui/`

4. Keep update order fixed:

- input
- player
- enemies
- collisions
- sound dispatch
- transition checks

5. Call feature resets before scene transitions when required.
6. Keep render differential and never flush (`sp1_UpdateNow`) inside scene module.

## Output

When complete, report:

1. Scene files created/updated.
2. Feature callbacks orchestrated and update order.
3. Transition/reset behavior.
4. Integration points used by scene dispatcher.
