---
description: "Use when implementing gameplay architecture in project templates. Defines responsibilities between main, scene, entities, and transversal services."
applyTo: "templates/**/src/**/*.c, templates/**/src/**/*.h"
---

# Project Scene Architecture

Minimal mandatory contract only. Keep workflows in skills.

## 1. Source and Ownership

- `main.c` is frame orchestrator only.
- Shared run-level state is owned by `src/game_state.c/.h`.
- `src/core/` owns reusable infra/services.
- `src/data/` owns asset declarations.
- `src/scenes/` owns orchestration, scene flow, and scene-local features.

## 2. Scene and Feature Placement

- Gameplay implementations: `src/scenes/gameplay/`.
- Interactive scenes (menu/game over): `src/scenes/`.
- Entity features: `src/scenes/features/entities/`.
- UI features: `src/scenes/features/ui/`.

## 3. Rendering and State Rules

- Keep one frame flush in central loop (`sp1_UpdateNow()`).
- Use differential rendering only.
- No scene/entity module initializes global game state directly.
- Cross-entity interaction goes through scene orchestration or transversal services.

## 4. Use Skills for Workflows

- `scene-entity-architecture`
- `create-gameplay-scene`
- `create-interactive-scene`
- `create-entity-feature`
- `create-ui-feature`
