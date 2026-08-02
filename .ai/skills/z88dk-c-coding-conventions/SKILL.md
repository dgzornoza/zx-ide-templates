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
- **Enum storage is the smallest fitting type by default:** SDCC selects the underlying integer type for an enum as the first of `unsigned char`, `signed char`, `unsigned int`, `signed int`, `unsigned long int`, `signed long int`, ... that fits all enumerator constants (see SDCC manual §"Structures, unions, enumerations and bit-fields"). There is no `--short-enums` flag — this small-fitting behavior IS the default. So an enum whose constants fit in 0–255 occupies **1 byte** in memory without any pragma. Prefer typed `enum` fields over `uint8_t` in layout-sensitive structs (BSS, save-state layouts, `sizeof`-documented structs) when there is a stable domain: you gain type-safety at the API boundary (no `(uint8_t)` cast on assignment) at zero memory cost vs `uint8_t`. **Always verify empirically** via the `.map`/`.lis` after a build before committing a struct that documents per-field offsets, since the rule is a compiler detail not enforced by the C type system.
- **ROM vs RAM (`const`):** Mark read-only data (like tilemaps, sprites, text strings) as `const` so the compiler can place them in ROM instead of consuming precious RAM.

## 4. Scope and Encapsulation

- **Static by Default:** Any variable or function that is not strictly needed outside its `.c` file MUST be declared `static`. This prevents symbol clashes across the project and helps the compiler optimize.
- **Minimize Globals:** If a variable must be global, clearly define it in a specific state file (like `game_state.h`) rather than creating scattered `extern` variables throughout the project.

## 5. Hotpath Optimization Patterns (On Demand)

Apply these patterns when optimizing loops and critical rendering/update paths:

- Use `uint8_t` loop counters whenever the range fits 0-255.
- Prefer pointer walking over indexed addressing in tight loops.
- In inner loops, read immutable fields directly from global state structs instead of copying them to locals.
- Keep locals in inner loops only for values that mutate each iteration.
- **Do not replace N independent static variables with a single array indexed by a loop counter.** SDCC compiles each static to `ld a,(_sym+n)` with a fixed compile-time offset; loop-indexed array access requires runtime offset calculation (`ld hl,_base / add hl,bc` per iteration) and inflates code size noticeably. The `draw_map_region` global-config pattern optimizes field-by-name access, not dynamic indexing.
- **Global struct fields accessed by name compile to the same ASM as independent static variables.** `gs.left` becomes `ld hl, (_gs + 0)` because SDCC resolves the field offset at compile time. This means a struct with named fields is a free upgrade for API clarity and maintenance with no ROM/RAM cost vs separate statics — verified empirically in `input_manager.c` (2465 bytes with struct + batch setter, identical to the baseline with N statics).
- Validate with generated `.lis` output before and after changes. Estimated size deltas can be off by 2–3×; only the `.lis` is ground truth.

```c
/* Better: pointer walk + 8-bit counter */
const uint8_t *tile_ptr = tiles;
for (uint8_t i = 1; i <= 64; i++)
{
  sp1_TileEntry(i, tile_ptr);
  tile_ptr += 8U;
}
```

```c
/* Better: direct global struct field in inner loop */
for (uint8_t index = 0; index < draw_map_config.map_width; index++)
{
  const uint8_t *tile_data = draw_map_config.tiles_data + ((uint16_t)index * 8U);
  sp1_TileEntry(index + 1U, tile_data);
}
```

## 6. Bitmask Flags for Compact State

When a system tracks N independent boolean states (pressed inputs, active power-ups, pending events), use a single `uint8_t` with one bit per state and expose one getter that returns the bitmask. Consumers AND the relevant flag in their own code.

```c
typedef enum
{
    INPUT_FLAG_LEFT   = (1u << 0),
    INPUT_FLAG_RIGHT  = (1u << 1),
    INPUT_FLAG_UP     = (1u << 2),
    INPUT_FLAG_DOWN   = (1u << 3),
    INPUT_FLAG_FIRE1  = (1u << 4)
} InputFlags;

uint8_t input_get_pressed(void) __z88dk_fastcall;

/* Caller pattern: read once, test many */
const uint8_t pressed = input_get_pressed();
if (pressed & INPUT_FLAG_LEFT)  { /* ... */ }
if (pressed & INPUT_FLAG_RIGHT) { /* ... */ }
```

**Why this beats N independent getters**:
- RAM: N bytes → 1 byte.
- ROM: one 4-byte getter instead of N small functions.
- Call sites: one `input_get_pressed()` call + N inline ANDs is cheaper than N function calls, especially when the same frame tests multiple flags.
- Extending to N+1 actions only requires adding one bit to the enum and one OR in the poll — no new function, no new static.

**Tradeoff**: callers lose the ability to test one action with zero overhead — they always go through a bitwise AND. For hot loops testing one flag per iteration (rare in practice), N getters would win. Default to the bitmask unless profiling shows the opposite.

## 7. Setter API Patterns

For config-style state (bindings, options, layout) that the user changes rarely (setup, menus, rebind UI):

**Preferred — batch setter with a global struct**:
```c
typedef struct { uint16_t left, right, up, down, fire1; } InputBindings;
extern InputBindings input_bindings;

void input_set_keyboard_bindings(const InputBindings *bindings);  /* takes a pointer */
```

- The setter takes one pointer (fastcall-eligible in spirit, though actually 1-param stack).
- The struct lives in BSS (no initializer = zero cost at boot); a separate `static const` holds the defaults.
- Callers from gameplay build an `InputBindings` locally and pass it — useful for the rebind UI which can have multiple profiles.
- See `input_manager.c` for the canonical implementation.

**Avoid — single-action setter with `switch` on an enum**:
```c
void input_set_key_binding(InputAction action, uint16_t scancode)
{
    switch (action)
    {
        case INPUT_ACTION_LEFT:  /* ... */ break;
        case INPUT_ACTION_RIGHT: /* ... */ break;
        /* ... */
    }
}
```

- SDCC compiles `switch` over enum to a jump table in ROM (~10 bytes for 5 cases) + per-case branches.
- Called from `reset_defaults`, it pays prologue/epilogue once per call — N calls cost ~10× more than one batch call.
- Empirically: adding a setter unificado to `input_manager.c` inflated the binary from 2465 to 2721 bytes (+256) vs a batch setter with struct (same byte count as the baseline without Fire1).

**If the rebind UI needs an enum-driven API later**, add it as a wrapper that maps enum → batch call, not as the primary write path. The wrapper can be enabled per-scene without bloating the always-linked code.

## 8. Optimization Goal Prompting Rule

When the user requests implementation and does not specify optimization target, ask before coding:

- Optimize for code size
- Optimize for execution speed

Do not assume both simultaneously in hotpaths; select one dominant objective and verify in `.lis` output.

## 9. Calling Conventions (sdcc_iy)

When declaring functions targeting the `sdcc_iy` backend, control how parameters are passed (registers vs. stack) with the right calling convention attribute.

|                        | __sdcccall(0) | __sdcccall(1) | __z88dk_fastcall |
|------------------------|---------------|---------------|------------------|
| **RETURN**             |               |               |                  |
| 8 bits                 | L             | A             | L                |
| 16 bits                | HL            | DE            | HL               |
| 32 bits                | DE-HL         | HL-DE         | DE-HL            |
| **PARAMETERS**         |               |               |                  |
| 8 bits                 | Stack         | A             | L                |
| 16 bits                | Stack         | HL            | DE               |
| 32 bits                | Stack         | HL-DE         | DE-HL            |
| 8 + 8 bits             | Stack         | A + L         | Invalid          |
| 8 + 16 bits            | Stack         | A + DE        | Invalid          |
| 16 + 8 bits            | Stack         | HL + DE       | Invalid          |
| 16 + 16 bits           | Stack         | HL + Stack    | Invalid          |

### Description

- **__sdcccall(0)** — Legacy convention (default until SDCC 4.1.11).
- **__sdcccall(1)** — New convention (default from SDCC 4.1.12).
- **__z88dk_fastcall** — Alternative convention that only works with **single-parameter** functions.

### Additional Explanation

`__sdcccall(1)` handles more cases than `__z88dk_fastcall` and also works with multi-parameter functions. Parameters not covered by the register rules fall through to the stack.

For example, a function taking **three `uint8_t` parameters** passes the first two through registers (**A** and **L**) and the third through the stack.

### Practical Implications

- Apply `__z88dk_fastcall` **only** to functions with 0 or 1 parameter. The compiler rejects multi-parameter combinations.
- For multi-parameter APIs, prefer `__sdcccall(1)` (current default) to keep the first 1–2 parameters in registers and the rest on the stack.
- For config-style state with many fields, prefer a batch setter that takes a pointer to a global struct (see section 7) instead of either an N-parameter function or a `switch`-based per-action setter. The struct is accessed by name in hotpaths, costing zero extra bytes vs separate statics.
- The current codebase applies `__z88dk_fastcall` only to 0-or-1-parameter functions (`input_poll`, `input_get_pressed`, `input_set_mode`, `input_set_joystick_type`, `input_reset_defaults`, `read_joystick_state`). Follow that pattern.

## 10. sdcc_iy Stack Layout — Reading the `.lis`

When `sdcc_iy` emits assembly for a function, four idioms repeat and look strange until you know what they mean. **All four are `sdcc_iy`-specific**; they do not appear with `__sdcccall(0)` or `__sdcccall(1)` because those conventions do not reserve `IX` as a frame pointer. The canonical example is `input_poll` in `templates/fixed-shoot-em-up/src/core/input/input_manager.c:108`.

### 10.1 Frame pointer (`IX`) save/restore — mandatory

`sdcc_iy` reserves `IX` as the **caller's frame pointer**. Every function MUST:

```
push ix             ; save caller's IX
ld ix, #<framesize> ; placeholder — linker patches with local-storage size
add ix, sp          ; IX = SP + framesize (pointing into caller's frame)
... function body uses (ix-N) for parameters and locals ...
ld sp, ix           ; restore SP from IX (also deallocates locals)
pop ix              ; restore caller's IX
ret
```

There is no escape hatch: omitting the `push ix` / `pop ix` would clobber the caller's frame pointer and silently break every local access in the caller's code path. Every `__z88dk_fastcall` function in this template pays this cost (see e.g. `input_manager.c:108`, `read_joystick_state:92`).

### 10.2 Stack reservation via `push af` / `push hl`

Z80 cannot decrement `SP` in a single instruction, so SDCC allocates local storage by pushing scratch registers. Each `push rr` reserves 2 bytes. Values pushed are **discardable garbage** — `AF` (or whichever scratch register SDCC chose) is not preserved across calls.

In `input_poll`:

```
push af        ; SP-=2  ┐
push af        ; SP-=2  ├ 3 pushes reserve 6 B total
push af        ; SP-=2  ┘
ld  (ix-1), l  ; store the playerId parameter at the first reserved byte
```

**How many bytes were reserved?** Count pushes × 2. For `input_poll`:

| Offset     | Source               | Size |
|------------|----------------------|------|
| `(ix-1)`   | `playerId` parameter | 1 B  |
| `(ix-2)`   | `mode` (cached)      | 1 B  |
| `(ix-3..4)`| `player` pointer     | 2 B  |
| `(ix-5..6)`| `player` pointer (duplicate) | 2 B |

6 reserved, 6 used. `flags` lives in register `E` for the whole function — see `set 0/1/2/3/4, e` in the `.lis`. The "duplicate" pointer is intentional (§10.3 explains why).

### 10.3 The pop-push idiom (cheap 16-bit local load)

When a 16-bit local happens to sit at the current `SP` (true for every local reserved via `push` and not overwritten by `ld (ix-N), r`), SDCC loads it with a 1-byte `pop rr` instead of a 3-byte `ld c,(ix-N); ld b,(ix-N+1)`. To keep `SP` consistent for the rest of the function, the loaded value is pushed back immediately:

```
pop hl              ; 1 byte — load local into HL
... use HL ...
push hl             ; 1 byte — restore SP (and re-store the local)
```

When two 16-bit locals are needed at once and SDCC wants them in two registers (e.g. `HL` + `BC`), the idiom doubles:

```
pop hl              ; load local A into HL (1 B)
pop bc              ; load local B into BC (1 B)
push bc             ; restore SP+2 (1 B)
push hl             ; restore SP+0 (1 B)
```

In `input_poll`, the function stores the `player` pointer **twice** — at `(ix-6)/(ix-5)` and at `(ix-4)/(ix-3)` — specifically so both `HL` and `BC` can be `pop`-loaded (4 B total) instead of doing two `ld rr,(ix-N)` pairs (12 B). The duplicate store costs 8 B up front but saves net bytes because `player` is used 7 times (mode + 5 bindings + joystick).

**When you see a pop-push in a `.lis`:**

- The `pop` values are **NOT** garbage — they are the function's just-stored locals. Verify by comparing against `ld (ix-N), r` writes in the prologue.
- Each `pop` outside the epilogue is a deliberate local load via stack, not register restoration.
- The trailing `push` keeps `SP` consistent for subsequent stack-relative accesses (`add hl, sp; ...`).

### 10.4 Reading a `.lis` prologue — checklist

When facing a new `__z88dk_fastcall` function in a `.lis`, walk this:

1. **`push ix; ld ix, #0; add ix, sp`** — frame pointer setup. Mandatory in `sdcc_iy`.
2. **Pushes of `af`/`hl`/`bc`/`de`** — count them. Bytes reserved = pushes × 2.
3. **`ld (ix-N), r` immediately after pushes** — initial local assignment, typically a parameter like `playerId`.
4. **Repeated `ld (ix-N), l; ld (ix-N+1), h` pairs** — local stores. Skip the duplicated pairs unless an idiom in (3) or pop-push needs them — they signal a peephole choice (see §10.3), not wasted bytes.
5. **`pop rr` outside the epilogue** — paired local loads; look for the matching `push rr` shortly after.
6. **`ld sp, ix; pop ix; ret`** at the end — mandatory epilogue.

If the prologue does NOT match step 1 (no `push ix`), the function is using `__sdcccall(0)` or `__sdcccall(1)` instead — check the function's attribute, not the prologue shape.

## 11. Commenting Discipline

Comments are noise unless they encode information that the code itself cannot.

**No function comments.** Do not write a comment above or inside a function that rephrases what it does. The function name is the documentation. A reader who needs to know what `menu_redraw_row` does should read the code, not a comment that says "redraws one menu row".

**No file-header blocks.** Do not write a top-of-file comment describing what the module contains, what scene it implements, or what the frame contract is. The includes, the function names, and the skill files are the documentation. File headers become stale the moment the code changes — once a file's behaviour shifts and the header is not updated, the header lies. The code cannot lie; a comment can.

**Only when non-obvious.** Add a comment only when explaining something the code cannot express directly: an architectural invariant that callers rely on, a non-portable Z80 trick, a memory-layout assumption verified by reading the `.lis`, a deliberate deviation from a rule. If a future reader could delete the comment and the code would still make sense, the comment should not exist.

**Style.** When a comment is necessary, keep it to one short line. No multi-paragraph essays inside a function body. English only (see section 2).
