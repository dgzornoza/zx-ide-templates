---
name: sp1-rendering
description: "Game loop, SP1 graphics/rendering arrays, screen updates, and interrupt synchronization for ZX Spectrum with z88dk SP1."
---

# SP1 Rendering and Game Loop Rules

This file is the mandatory rendering/runtime contract.
Implementation templates are intentionally moved to skills to minimize default token load.

## 1. Frame Synchronization Contract

- The main loop must be synchronized to the 50Hz interrupt tick (IM2).
- Waiting between frames must yield CPU (`intrinsic_halt()` pattern).

## 2. Single Flush Contract

- Keep exactly one `sp1_UpdateNow()` per frame in the central frame loop.
- Do not call `sp1_UpdateNow()` from entity, UI, or service modules.

## 3. Differential Rendering Contract

- Full-screen clear/redraw is forbidden during normal gameplay frames.
- Renderers must update only tiles/sprites changed since the previous frame.

## 4. Ownership Contract

- Keep IM2 setup/ISR in infrastructure modules.
- Keep `main.c` free of IM2 internals.
- Keep assets under `data/` as extern declarations; do not embed raw asset arrays in logic modules.

## 5. Detailed Guidance Location

- For SP1 setup and rendering templates, use skill `z88dk-sp1-codegen`.
- For IM2 setup templates, use skill `z88dk-im2-setup`.