---
name: scene-entity-architecture
description: "Use when defining scene/feature architecture and deciding which specialized skill to invoke for gameplay scenes, interactive scenes, entity features, or UI features."
user-invocable: true
argument-hint: "Describe what you need to create (gameplay scene, interactive scene, entity feature, UI feature) and target files."
---

# Scene Feature Architecture Router

## When to Use

Use this skill first when you need to:

- Design scene boundaries before coding.
- Decide whether work belongs to gameplay scenes, interactive scenes, entity features, or UI features.
- Select the correct specialized skill for scaffolding.

## Source of Truth

For project architecture and ownership rules, follow:

- `.github/instructions/project-architecture.instructions.md`

For CPU and rendering constraints, follow global instructions:

- `.github/instructions/z80-c-architecture.instructions.md`
- `.github/instructions/sp1-rendering.instructions.md`

## Architecture Decisions

Evaluate decisions strictly in order `1 -> 2 -> 3 -> 4`. Each decision narrows context for the next one; do not re-evaluate prior decisions after moving forward.

1. Scene type decision:

- Gameplay scene implementation files must go in `src/scenes/gameplay/`.
- All source and header files for interactive non-gameplay scenes (for example menu or game over) must go in `src/scenes/`.

2. Feature type decision:

- Entity features must go in `src/scenes/features/entities/`.
- UI features must go in `src/scenes/features/ui/`.

3. Ownership decision:

- `main.c` orchestrates frame and scene dispatch only.
- Scene modules orchestrate order and transitions.
- Feature modules own local `static` state and expose only required lifecycle callbacks.

4. Lifecycle and update-order decision:

- Keep coherent naming: `init`, `update`, `render` only when consumed by orchestrator.
- Fixed gameplay update order:
	1. input
	2. player
	3. enemies
	4. collisions
	5. sound dispatch
	6. transition checks
- Before any scene transition, call `<entity>_reset(void)` for every entity type that uses a fixed-size pool (2 or more simultaneous instances).


## Lifecycle API Contract (Coherence + Size Rule)

This section defines architecture decision outputs; detailed implementation stays in the specialized creation skill selected later.

- Keep lifecycle naming coherent across gameplay modules (`init`, `update`, `render`) and expose only callbacks consumed by the owning orchestrator.
- This router does not define concrete function signatures for scene/entity/UI modules; the selected specialized skill defines exact API shapes.
- If a module does not need one lifecycle step and that callback is not called by its orchestrator, omit the declaration and implementation.
- Avoid mandatory no-op callbacks by default in 8-bit targets because each extra symbol and call site increases code size and frame cost.
- Use no-op callbacks only when a shared orchestration mechanism explicitly requires a fixed callback table.
- Optional APIs are allowed only by explicit rules in this file. `_reset` is the only optional API covered here and must follow the reset decision table in this document.

## Feature and Entity Ownership Rules

- Entity modules must live in `src/scenes/features/entities/`.
- UI feature modules must live in `src/scenes/features/ui/`.
- Each active gameplay entity must expose the lifecycle callbacks that are actually consumed by the scene orchestrator (typically `init`, `update`, and `render` for playable actors).
- Reset decision table:
  1. If an entity uses a fixed-size pool (2 or more simultaneous instances), include `<entity>_reset(void)`.
  2. If an entity is single-instance and initialized once per run, omit `<entity>_reset(void)`.
  3. No other cases permit `<entity>_reset(void)` unless a scene architecture decision in this skill explicitly states it.
- Feature state must be file-local with `static` variables.
- Use fixed-size pools for any entity type that can have 2 or more simultaneous instances.
- Define each pool size as a compile-time constant in the entity header.

## Specialized Skills Mapping

After decisions are locked, invoke exactly one specialized creation skill per work item:

- Use `create-gameplay-scene` for playable scenes in `src/scenes/gameplay/`.
- Use `create-interactive-scene` for menu/game over/interaction scenes in `src/scenes/`.
- Use `create-entity-feature` for gameplay entity features in `src/scenes/features/entities/`.
- Use `create-ui-feature` for HUD/score/lives and other UI features in `src/scenes/features/ui/`.

If a request spans multiple categories, decompose it into separate work items, report each item with its own architecture decision and skill mapping, and ask the user to confirm invocation order before continuing.

## Output

When complete, report:

1. Architecture decisions taken (scene type + feature type).
2. Selected specialized skill.
3. Expected target paths.
4. Lifecycle APIs expected by orchestrators.
