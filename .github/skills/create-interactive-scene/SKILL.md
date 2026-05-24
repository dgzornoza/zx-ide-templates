---
name: create-interactive-scene
description: "Use when creating non-gameplay interactive scenes such as main menu, game over, pause, or credits screens."
user-invocable: true
argument-hint: "Describe scene type (menu/game over/pause), inputs handled, and transition targets."
---

# Create Interactive Scene Workflow

## When to Use

Use this skill for interaction-first scenes that are not gameplay loops.

- Target folder: `src/scenes/`.
- Example files: `main_menu.c/.h`, `game_over.c/.h`.

## Constraints

Follow:

- `.github/instructions/project-architecture.instructions.md`
- `.github/instructions/z80-c-architecture.instructions.md`
- `.github/instructions/sp1-rendering.instructions.md`

## Procedure

1. Create or update scene module directly under `src/scenes/`.
2. Expose `<scene>_init(void)`, `<scene>_update(void)`, `<scene>_render(void)` only if needed by orchestrator.
3. Keep scene responsible for local interaction state and transition flags.
4. If UI elements are reusable across scenes, place them in `src/scenes/features/ui/` and call them from this scene.
5. Keep rendering differential and avoid calling `sp1_UpdateNow()` here.

## Output

When complete, report:

1. Scene files created/updated under `src/scenes/`.
2. Input handling and transition rules.
3. Reused or new UI features.
4. Dispatcher integration points.
