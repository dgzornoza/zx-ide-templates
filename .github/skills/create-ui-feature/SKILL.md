---
name: create-ui-feature
description: "Use when creating or refactoring UI features (HUD, score, lives, labels) under src/scenes/features/ui."
user-invocable: true
argument-hint: "Describe UI feature purpose, data dependencies, and owning scenes."
---

# Create UI Feature Workflow

## When to Use

Use this skill for scene-level UI features.

- Target folder: `src/scenes/features/ui/`.
- Examples: HUD, score panel, lives indicator, wave/level text.

## Constraints

Follow:

- `.github/instructions/project-architecture.instructions.md`
- `.github/instructions/z80-c-architecture.instructions.md`
- `.github/instructions/sp1-rendering.instructions.md`

## Procedure

1. Create `<ui_feature>.h/.c` under `src/scenes/features/ui/`.
2. Expose only callbacks required by owning scenes:

- `<ui_feature>_init(void)`
- `<ui_feature>_update(void)` if dynamic
- `<ui_feature>_render(void)`

3. Keep UI state file-local with `static` variables.
4. Read shared data from `game_state.h` for score/lives/scene values.
5. Perform differential updates only; avoid full redraws in gameplay frames.
6. Never call `sp1_UpdateNow()` in UI feature code.

## Output

When complete, report:

1. UI feature files created/updated.
2. Lifecycle callbacks exposed.
3. Shared state fields consumed.
4. Owning scenes wired to the feature.
