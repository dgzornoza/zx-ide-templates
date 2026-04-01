---
name: z88dk-c-coding-conventions
description: "Use when writing or refactoring C code for z88dk to ensure it follows the project's strict coding style, optimization rules, and readability conventions."
user-invocable: true
---

# Z88DK C Coding Conventions

When writing C code for the Z80 (ZX Spectrum) using z88dk, adhere strictly to the following conventions for both readability and 8-bit performance.

## 1. Block Formatting (Braces)

- **Always use braces:** All control structures (`if`, `else`, `while`, `for`, `switch`) MUST use curly braces `{}`, even if the block contains only a single line. Never omit braces.
- **Brace placement:** Follow the existing project style, placing braces on a new line for functions, structs, and control structures if that matches the surrounding code.
- **Example:**

  ```c
  // GOOD
  if (player_health > 0)
  {
      player_health--;
  }

  // BAD (Never do this)
  if (player_health > 0) player_health--;
  ```

## 2. Naming Conventions (No Acronyms)

- **Descriptive Names:** ALWAYS use full, descriptive words for variables and functions. Never use acronyms or ambiguous abbreviations (e.g., use `player_health` instead of `p_hp`, `maximum_enemies` instead of `max_en`).
- **Exceptions:** Standard loop iterators like `i`, `j`, `k`, and coordinate variables like `x`, `y`, `z` are allowed and encouraged for short, localized loops or spatial logic.
- **English Only:** All names, comments, and documentation must be written in English.
- **Case conventions:**
  - Variables and functions: `snake_case` (e.g., `game_score`, `update_hud`).
  - Macros, constants, and enums: `UPPER_SNAKE_CASE` (e.g., `MAX_PLAYERS`, `STATE_PLAYING`).
  - Types/Structs: `PascalCase` or `camelCase` (e.g., `GameState`).

## 3. Types and 8-bit Optimization

- **Use `<stdint.h>` types:** Always use explicit fixed-width integers (`uint8_t`, `int8_t`, `uint16_t`, `int16_t`). Do not use standard `int` or `char` without explicit sign/size formatting.
- **8-bit preferred:** The Z80 is an 8-bit processor. 16-bit math is slower and takes more code space. Use `uint8_t` or `int8_t` for loops, array indexing, and counters whenever the maximum value fits in 0-255.
- **ROM vs RAM (`const`):** Mark read-only data (like tilemaps, sprites, text strings) as `const` so the compiler can place them in ROM instead of consuming precious RAM.

## 4. Scope and Encapsulation

- **Static by Default:** Any variable or function that is not strictly needed outside its `.c` file MUST be declared `static`. This prevents symbol clashes across the project and helps the compiler optimize.
- **Minimize Globals:** If a variable must be global, clearly define it in a specific state file (like `game_state.h`) rather than creating scattered `extern` variables throughout the project.
