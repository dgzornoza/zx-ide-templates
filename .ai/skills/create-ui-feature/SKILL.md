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

- `.ai/rules/project-architecture.md`
- `.ai/rules/z80-c-architecture.md`
- `.ai/rules/sp1-rendering.md`

## Procedure

1. Create `<ui_feature>.h/.c` under `src/scenes/features/ui/`.
2. Expose only callbacks required by owning scenes:

- `<ui_feature>_init(void)` - one-shot setup (e.g., `sp1_TileEntry` registrations, static asset load).
- `<ui_feature>_update(void)` - per-frame tick. The function MUST register its SP1 dirty-marks here (e.g., `sp1_PrintAt*` for state-driven widgets). Gate the dirty-marker work by a state-change check when the widget is mostly static - this keeps the SP1 update list small without introducing a separate `_render` callback.

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
