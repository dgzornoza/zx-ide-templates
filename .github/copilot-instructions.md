# AI Coding Instructions for zx-ide-templates

## Project Overview

This workspace contains **ZX Spectrum game development templates** using the z88dk compiler and SP1 graphics engine.

- **Target:** ZX Spectrum (8-bit retro platform)
- **Hardware constraints:** 48K addressable RAM, no hardware multiply/divide, no floating point, 256x192 display. If a requested feature exceeds these limits, state the constraint explicitly and propose the closest feasible z88dk/SP1 alternative.
- **Language:** C + Z80 Assembly (via z88dk/SDCC)
- **Graphics Engine:** SP1 (differential update sprite engine)
- **Build System:** Makefile-based (z88dk compiler toolchain)
- **Reference docs:** `docs/z88dk-sp1.md`

## Core Language Rules

- **English Only**: ALL variables, function/method names, and code comments MUST ALWAYS be written in English.

## Maintenance Rule

- Any change to `.github/instructions/` or `.github/skills/` MUST update this file in the same change to keep the guidance index synchronized.

## Global Z88DK/Z80 & SP1 Rules (Instructions)

The following guidelines dictate the architecture of the game. They are loaded automatically by Copilot when relevant.
Instructions in `./instructions/` are mandatory constraints and cannot be overridden. Skills in `./skills/` define workflows within those constraints. If a skill output would violate an instruction, the instruction takes precedence and the conflict must be surfaced to the user.

- **[Z80 C Architecture Guidelines](./instructions/z80-c-architecture.instructions.md)** (Global Z88DK/Z80): Minimal mandatory contract (memory, state, module ownership) for all C code.
- **[SP1 Rendering & Game Loop](./instructions/sp1-rendering.instructions.md)** (SP1 Specific): Minimal mandatory runtime/render contract (single flush, differential rendering, IM2 ownership).
- **[Project Scene Architecture](./instructions/project-architecture.instructions.md)** (Project Templates): Compact mandatory contract for source-tree hierarchy, scene/feature ownership, lifecycle boundaries, and update order. Workflow templates live in skills.

## Agent Skills

- When multiple skills apply, use this priority: 1) `/scene-entity-architecture` for architecture decisions, 2) `/create-gameplay-scene` for playable scene scaffolding, 3) `/create-interactive-scene` for non-gameplay scenes, 4) `/create-entity-feature` for entity features, 5) `/create-ui-feature` for UI features, 6) `/z88dk-im2-setup` for interrupt setup, 7) `/z88dk-sp1-codegen` for SP1 implementation patterns, 8) `/z88dk-c-coding-conventions` for C style/performance pass.
- **`/scene-entity-architecture`**: Use the **[Scene Feature Architecture Router Skill](./skills/scene-entity-architecture/SKILL.md)** to classify work by scene/feature type and pick the right specialized skill.
- **`/create-gameplay-scene`**: Use the **[Gameplay Scene Creation Skill](./skills/create-gameplay-scene/SKILL.md)** when creating or refactoring playable scenes under `src/scenes/gameplay/`.
- **`/create-interactive-scene`**: Use the **[Interactive Scene Creation Skill](./skills/create-interactive-scene/SKILL.md)** for menu/game over/pause/credits style scenes under `src/scenes/`.
- **`/create-entity-feature`**: Reuse the **[Gameplay Entity Feature Skill](./skills/create-entity-feature/SKILL.md)** for entities under `src/scenes/features/entities/`.
- **`/create-ui-feature`**: Use the **[UI Feature Skill](./skills/create-ui-feature/SKILL.md)** for HUD/score/lives and related modules under `src/scenes/features/ui/`.
- **`/z88dk-c-coding-conventions`**: Use the **[Z88DK C Coding Conventions Skill](./skills/z88dk-c-coding-conventions/SKILL.md)** whenever writing, reviewing, or refactoring C code to apply style rules plus hotpath optimization patterns (`.lis`-driven).
- **`/z88dk-sp1-codegen`**: Use the **[SP1 Code Generation Skill](./skills/z88dk-sp1-codegen/SKILL.md)** whenever you need implementation templates for SP1 initialization, sprites, rendering loops, or performance-oriented SP1 patterns.
- **`/z88dk-im2-setup`**: Use the **[IM2 Setup Skill](./skills/z88dk-im2-setup/SKILL.md)** whenever you need to implement or refactor IM2 setup, either with SP1 (`0xD0`/`0xD1` layout) or without SP1 (general z88dk aligned-page layout).

---

## Templates

### `templates/fixed-shoot-em-up`

A fixed vertical shooter game template. This is the primary active template.

#### Project Structure

```
fixed-shoot-em-up/
├── Makefile
├── zpragma.inc
├── bin/                        # Build output (.sna, .tap, .lis)
├── obj/                        # Intermediate object files
└── src/
    ├── main.c                  # Entry point and frame orchestration only
    ├── game_state.c/.h         # Shared run-level game state
    ├── z88dk_headers.h         # Centralized z88dk header includes
    ├── core/                   # Reusable infrastructure/services (input, ISR, utils)
    ├── data/                   # Asset declarations (extern tiles/sprites/tilemaps)
    └── scenes/                 # Scene orchestration, gameplay scenes, and features
```

#### Architecture Conventions

- `main.c` MUST contain only: `#include` directives and the `main()` function with setup calls plus the game loop orchestration. No gameplay logic and no subsystem implementation code.
- Cross-cutting concerns (interrupts, hardware init, input helpers, utility drawing helpers) go in `src/core/`.
- Asset data declarations and generated data bindings go in `src/data/`.
- Scene composition, interactive screens, gameplay levels, and scene features go in `src/scenes/`.
- Prefer explicit hierarchy (`core`, `data`, `scenes`) instead of many unrelated root folders under `src/`.
- Coherence contract for gameplay modules: keep `init`, `update`, `render` naming, but implement only callbacks required by the orchestrator; avoid no-op methods unless a shared callback table requires them.
- The Makefile auto-discovers all `.c` and `.asm` files under `src/` recursively — no Makefile changes needed when adding files.

#### Build

```bash
make COMPILER=sdcc CLIB=sdcc_iy CRT=31 C_OPT_FLAGS="-SO3 --opt-code-size" CREATE_SNA=true CREATE_TAP=true
```

Or use the VS Code **Build** task (default build task).

#### Key `zpragma.inc` settings

```c
#pragma output CRT_ORG_CODE = 24500   // Code origin
#pragma output REGISTER_SP  = 0xd000  // Stack pointer (required for SP1 + IM2)
```

> `REGISTER_SP = 0xd000` is why IM2 uses `0xD0`/`0xD1` for the vector table and jump point — the SP1 engine relocates the stack there, making that region safe and unused.

---
