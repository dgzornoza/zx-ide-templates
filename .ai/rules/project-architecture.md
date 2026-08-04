---
name: project-architecture
description: "Source-tree hierarchy, scene/feature ownership, lifecycle boundaries, and update order for project templates."
globs:
  - "templates/**/src/**/*.c"
  - "templates/**/src/**/*.h"
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

## 5. C ↔ Z80 Assembly Companion Layout

When a header declares symbols implemented in Z80 assembly, the source tree MUST mirror this layout:

```text
<owner>/
├── <feature>.h           # public C interface; declares externs only
└── <feature>/            # sibling folder, SAME basename as the .h
    ├── <feature>.asm     # exactly one C-callable routine per file
    └── globals.asm       # shared/global assembly variables for the feature
```

Rules:

- `<feature>.h` MUST contain only C declarations (`extern` prototypes and type definitions). It MUST NOT contain `.c` or `.asm` bodies.
- Each `extern` in `<feature>.h` MUST have exactly one `.asm` file inside `<feature>/` that defines it, named identically to the C symbol it implements (without the leading underscore).
- One C-callable routine per `.asm` file. Do not co-locate multiple routines in one file.
- Shared assembly variables for the feature go in `<feature>/globals.asm` (one file per feature, not per variable). Each variable MUST be declared `PUBLIC` with the leading underscore.
- Symbol naming: C `extern` names MUST NOT carry the leading underscore. Assembly `PUBLIC` names and `EXTERN` references MUST carry the leading underscore (sdcc prepends `_` to C identifiers).
- The companion folder MUST contain only `.asm` files for that feature. Do not place `.c` or `.h` inside it.
- The companion folder MAY be omitted when a feature has no assembly symbols (pure-C features keep `.c` next to `.h`).
- A neutral folder named `asm/` (or any folder whose name does not match its companion header basename) MUST NOT be used to hold loose `.h`/`.asm` pairs. Each feature with assembly symbols MUST use its own `<feature>/` folder matching the basename of its public header.

Canonical example:

```text
src/core/infrastructure/
├── timer.h
└── timer/
    ├── timer.asm       # _update_timer
    └── globals.asm     # _GLOBAL_TIMER_TICKS, _GLOBAL_TIMER_ABS_TICKS, _GLOBAL_TIMER_PAUSE
```

Rationale: keeps the public interface (the `.h`) and the assembly implementation (the folder) discoverable from a single location, isolates each routine for incremental assembly, and makes sdcc ↔ asm symbol pairing explicit by co-locating files with matching names.