---
description: "Use when writing C code, game logic, entities, or state management for the Z80 architecture (ZX Spectrum). Rules for performance optimization."
applyTo: "**/*.c, **/*.h"
---

# Z80 C Architecture Rules

When writing C code for the Z80 (ZX Spectrum) processor, follow these strict architectural rules for performance and memory optimization:

## 1. No Dynamic Memory

- NEVER use `malloc`, `calloc`, or `free`. The heap overhead is too slow and causes fragmentation.
- Pre-allocate all entities and elements using static arrays of fixed sizes.
- Enable or disable entities using an `is_active` boolean flag.

## 2. Zero-Parameter Functions

- Function arguments are pushed to the stack, which is extremely slow on the Z80.
- Avoid passing multiple parameters to functions (e.g., `void update_enemy(int x, int y)` is forbidden).
- Instead, use file-scoped `static` global variables, and have the functions read them directly.

## 3. Global State Machine

- Game flow and shared data (like score, lives, current level) must be centralized in a global state machine (e.g., `game_state.h`).
- Modules should `#include "state/game_state.h"` and read/write the variables directly to avoid parameter passing overhead.

## 4. File Responsibilities

- Keep hardware concerns (interrupt setup, low-level initialization) inside `src/infrastructure/`.
- Keep `main.c` as an orchestrator that calls public setup/update entry points only.

## 5. IM2 Interrupt Pattern (General Z88DK)

When creating interrupts without SP1, set up the IM2 vector table in a safe, aligned 256-byte page that does not overlap stack, heap, asset buffers, or library-reserved memory.

- A common choice is a page like `0xFE` when available in your memory map.
- Isolate all ISR definitions and setup inside an `infrastructure/` file.
- The main code should only invoke a clean `im2_setup()` interface.
- For ready-to-use IM2 templates (with and without SP1), use skill `z88dk-im2-setup`.
