---
description: "Use when writing C code, game logic, entities, or state management for the Z80 architecture (ZX Spectrum). Rules for performance and memory optimization."
applyTo: "**/*.c, **/*.h"
---

# Z80 C Architecture Rules

This file is the mandatory baseline contract for all C code in templates.
Deep optimization recipes and code patterns are intentionally moved to skills to avoid loading extra tokens by default.

## 1. Assembly-First Baseline

- Every C construct becomes Z80 assembly; code size is the default optimization target unless the user explicitly asks for speed.
- If optimization goal is not stated, stop and ask whether to optimize for code size or execution speed before implementing.

## 2. Memory and Function Call Contract

- Never use dynamic allocation (`malloc`, `calloc`, `free`) in gameplay code.
- Pre-allocate entities/elements with fixed-size static pools.
- Avoid multi-parameter hot-path functions; use module-local `static` state and shared run-level state instead.

## 3. Shared State Contract

- Shared run-level data must be centralized in the game state module and accessed from there.
- Do not scatter independent `extern` state across unrelated modules.

## 4. Module Responsibility Contract

- Keep hardware concerns (interrupt setup, low-level initialization) in the infrastructure layer.
- Keep `main.c` as orchestrator only.

## 5. Detailed Guidance Location

- For C style and optimization patterns, use skill `z88dk-c-coding-conventions`.
- For SP1 rendering templates, use skill `z88dk-sp1-codegen`.
- For IM2 setup templates, use skill `z88dk-im2-setup`.
