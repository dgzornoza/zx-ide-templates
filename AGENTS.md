# AGENTS.md — zx-ide-templates

> Cross-agent master entry point.
> Read this first if you are an AI coding agent working in this repository.
> For full layout and maintainer conventions, see `.ai/README.md`.

## Project snapshot

`zx-ide-templates` ships ready-to-use game templates for **ZX-IDE**, a VS Code extension for retro ZX Spectrum development. The active template is `templates/fixed-shoot-em-up`, which targets the **ZX Spectrum 48K** and uses the **z88dk** toolchain (SDCC backend, `sdcc_iy`) with the **SP1** graphics engine.

- **Language:** C + Z80 assembly (via z88dk/SDCC).
- **Build:** Makefile-based. See `.ai/README.md` for the canonical command.
- **License:** AGPL-3.0-or-later. Contributions require `CLA.md` and follow `CONTRIBUTING.md`.

## Hard constraints

These constraints are non-negotiable for any code change in this repo. They mirror the rules under `.ai/rules/`.

- **Memory:** 48K addressable RAM. No dynamic allocation; use static pools and globals.
- **CPU:** No hardware multiply/divide, no floating point. Stack is ~128 bytes; never pass structs by value.
- **Display:** 256×192 pixels, 8×8 attribute cells, single color per attribute.
- **Single flush:** One frame flush per central loop (`sp1_UpdateNow()`). Differential rendering only.
- **IM2 ownership:** IM2 is owned by the central loop; SP1 and ISR cooperate with the 50Hz tick.
- **English only:** All identifiers, comments, and code MUST be in English.

If a requested feature exceeds these limits, state the constraint explicitly and propose the closest feasible z88dk/SP1 alternative.

## Skill index

Skills are workflow templates under `.ai/skills/<name>/SKILL.md`. Load the skill that matches the work.

| Skill | When to load |
|---|---|
| `scene-entity-architecture` | Defining scene/feature architecture and deciding which specialized skill to invoke. |
| `create-gameplay-scene` | Creating or refactoring playable gameplay scenes under `src/scenes/gameplay`. |
| `create-interactive-scene` | Creating non-gameplay interactive scenes (menu, game over, pause, credits). |
| `create-entity-feature` | Creating entity features (player, enemies, bullets, pickups) under `src/scenes/features/entities`. |
| `create-ui-feature` | Creating UI features (HUD, score, lives, labels) under `src/scenes/features/ui`. |
| `z88dk-c-coding-conventions` | Writing or refactoring C code for z88dk (style + hotpath optimization). |
| `z88dk-im2-setup` | Creating or refactoring IM2 interrupt setup, with or without SP1. |
| `z88dk-sp1-codegen` | Implementing SP1 sprites, tiles, collisions, render loops, performance patterns. |

Priority order when multiple skills apply: `scene-entity-architecture` first, then the relevant creation skill, then `z88dk-im2-setup` / `z88dk-sp1-codegen`, then `z88dk-c-coding-conventions` for style passes.

## Rule index

Rules are mandatory contracts under `.ai/rules/<name>.md`. They take precedence over skill output.

| Rule | When it applies |
|---|---|
| `project-architecture` | Source-tree hierarchy, scene/feature ownership, lifecycle boundaries, update order. |
| `sp1-rendering` | Game loop, SP1 graphics arrays, screen updates, interrupt sync. |
| `z80-c-architecture` | Writing C code, game logic, entities, state management on Z80. |

If a skill output would violate a rule, the rule wins and the conflict MUST be surfaced to the user.

## Migration note

The previous GitHub-Copilot-shaped guidance surface (`.github/copilot-instructions.md`, `.github/instructions/*.instructions.md`, `.github/skills/`) was retired as part of this refactor. The canonical entry point is this file; the canonical layout lives under `.ai/`. If you arrived from a legacy Copilot path, this is your new home.

## Where to go next

- **Agents looking for workflow templates:** open `.ai/skills/<name>/SKILL.md`.
- **Agents looking for mandatory contracts:** open `.ai/rules/<name>.md`.
- **Maintainers adding new skills or rules:** see `.ai/README.md` for layout and conventions.