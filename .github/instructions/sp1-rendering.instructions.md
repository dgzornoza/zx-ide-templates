---
description: "Use when modifying the game loop, handling SP1 graphics/rendering arrays, and synchronizing screen updates and interrupts."
---

# SP1 Rendering and Game Loop Rules

## 1. 50Hz Game Loop (IM2)

- The main game loop is synchronized to the 50Hz vertical blank using IM2 interrupts.
- A global `volatile uint8_t frame_tick` is incremented inside the `IM2_DEFINE_ISR(isr)`.
- The main loop uses `intrinsic_halt()` while waiting for `frame_tick` to change. This puts the Z80 to sleep until the next interrupt, saving CPU cycles and leaving the bus free.

### IM2 Addressing with SP1

- When SP1 is active and `REGISTER_SP = 0xd000`, prefer the IM2 layout based on `0xD0`/`0xD1` pages.
- Keep IM2 setup and ISR implementation in `src/infrastructure/`.
- Keep `main.c` free of IM2 internals: it should only call a public setup function such as `im2_setup(void)`.
- For code scaffolding and exact templates, use skill `z88dk-im2-setup`.

## 2. Rendering Optimization

- NEVER clear and redraw the entire screen every frame. The Z80 is too slow for that.
- Keep track of the "last drawn state" inside UI or entity renderers (e.g., `static uint16_t last_drawn_score`).
- Compare the current global state (e.g., `game_score`) with the last drawn state. Only invoke `sp1_PrintAt` or tile update functions if the value has actually changed.

## 3. Assets Separation

- Assets are separated in the `data/` folder and included as `extern` arrays.
- Do not embed or define raw asset arrays inside logic C files.

## 4. SP1 Initialization Rule

- Initialize SP1 once during startup.
- Invalidate the screen before the first frame.
- Render/update through a single `sp1_UpdateNow()` call per frame.
- For complete initialization templates, use skill `z88dk-sp1-codegen`.
