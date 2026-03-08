# AI Coding Instructions for zx-ide-templates

## Project Overview

This workspace contains **ZX Spectrum game development templates** using the z88dk compiler and SP1 graphics engine.

- **Target:** ZX Spectrum (8-bit retro platform)
- **Language:** C + Z80 Assembly (via z88dk/SDCC)
- **Graphics Engine:** SP1 (differential update sprite engine)
- **Build System:** Makefile-based (z88dk compiler toolchain)
- **Reference docs:** `docs/z88dk-sp1.md`

---

## Templates

### `templates/fixed-shooter-up`

A fixed vertical shooter game template. This is the primary active template.

#### Project Structure

```
fixed-shooter-up/
├── Makefile
├── zpragma.inc
├── bin/                        # Build output (.sna, .tap, .lis)
├── obj/                        # Intermediate object files
└── src/
    ├── main.c                  # Entry point – calls setup functions, runs game loop
    ├── z88dk_headers.h         # Centralized z88dk header includes
    └── infrastructure/
        ├── isr.c               # IM2 ISR definition and im2_setup()
        └── isr.h               # Public interface: void im2_setup(void)
```

#### Architecture Conventions

- `main.c` must stay **clean**: only calls setup functions and runs the game loop.
- Cross-cutting concerns (interrupts, hardware init) go in `src/infrastructure/`.
- Domain logic (game entities, input, rendering) goes in dedicated `src/<domain>/` subfolders.
- Each subsystem exposes a single `.h` header with its public interface.
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

#### IM2 Interrupt Pattern (SP1 layout)

IM2 uses `0xD0` and `0xD1` because SP1 relocates the stack to `0xD000`, making `0xD000–0xD1FF` a safe, unused region.

```c
// infrastructure/isr.c
#define TABLE_HIGH_BYTE      ((unsigned int)0xD0)
#define JUMP_POINT_HIGH_BYTE ((unsigned int)0xD1)
#define TABLE_ADDR  ((void *)(TABLE_HIGH_BYTE * 256u))
#define JUMP_POINT  ((unsigned char *)((unsigned int)(JUMP_POINT_HIGH_BYTE * 256u) + JUMP_POINT_HIGH_BYTE))

IM2_DEFINE_ISR(isr) { /* 50 Hz game tick logic here */ }

void im2_setup(void) {
    memset(TABLE_ADDR, JUMP_POINT_HIGH_BYTE, 257);
    z80_bpoke(JUMP_POINT, 195);
    z80_wpoke(JUMP_POINT + 1, (unsigned int)isr);
    im2_init(TABLE_ADDR);
    intrinsic_ei();
}
```

`main.c` only calls `im2_setup()` — never contains IM2 implementation details.

#### SP1 Initialization Pattern

```c
struct sp1_Rect full_screen = {0, 0, 32, 24};
sp1_Initialize(SP1_IFLAG_MAKE_ROTTBL | SP1_IFLAG_OVERWRITE_TILES | SP1_IFLAG_OVERWRITE_DFILE,
               INK_WHITE | PAPER_BLACK, ' ');
sp1_Invalidate(&full_screen);

while (1) {
    sp1_UpdateNow();
}
```

---

## Key Conventions (all templates)

### Memory Constraints

- ZX Spectrum has only **48KB RAM** (128KB on extended models)
- Avoid `malloc()` / dynamic allocation except via SP1's internal allocator (`CLIB_BALLOC_TABLE_SIZE`)
- Use **global variables** and `static` declarations — stack is tiny (128 bytes), no deep recursion
- Prefer `uint8_t`, `uint16_t`, `int8_t` (`<stdint.h>`) over `int` in hot paths

### Code Style

- Use `__z88dk_fastcall` for single-argument functions (passes arg in `HL`, reduces generated code)
- Declare constant data as `const` to keep it in ROM and save RAM
- Never use `printf()` in game loops — use `sp1_PrintAtInv` or `sp1_PrintString`
- Attribute clash: colors are per 8×8 cell, not per pixel — design sprites accordingly

### File Naming

- Infrastructure/hardware layer: `src/infrastructure/<name>.c` + `src/infrastructure/<name>.h`
- Game domain modules: `src/<domain>/<name>.c` + `src/<domain>/<name>.h`

---

## Available Skills

Skills extend Copilot with specialized domain knowledge. Load them by referencing the skill description.

| Skill | File | When to use |
|-------|------|-------------|
| **z88dk-sp1-codegen** | `.github/skills/z88dk-sp1-codegen/SKILL.md` | Generate C/ASM code for SP1 sprites, tiles, UDGs, game loops, collisions, IM2, zpragma.inc, module scaffolding |
| **find-skills** | `.github/skills/find-skills/SKILL.md` | Discover and install new agent skills from the skills ecosystem |

### z88dk-sp1-codegen

Use when:
- Creating/extending sprites (masked, animated, multi-column)
- Initializing SP1 or configuring `zpragma.inc`
- Implementing game loop, input handling, collisions
- Scaffolding new modules following project conventions
- Optimizing for 8-bit memory constraints

### find-skills

Use when the user asks "is there a skill for X" or wants to extend agent capabilities.

### Callback Pattern

Polling functions receive a `__z88dk_fastcall` function pointer for tight loops:

```c
void wait_for_intro_key(void (*callback)(void)) __z88dk_fastcall;
// Called from modules to wait for INTRO key while running callback each frame
```

## Build & Debugging Commands

| Task              | Command                                                                                      |
| ----------------- | -------------------------------------------------------------------------------------------- |
| **Build**         | `make COMPILER=sdcc CLIB=sdcc_iy CRT=1 CREATE_SNA=true CREATE_TAP=true` (VS Code Build task) |
| **Clean**         | `make clean_all` (removes bin/ obj/ artifacts)                                               |
| **Emulator**      | Output `.sna` snapshot runs in any ZX Spectrum emulator (Fuse, ZEsarUX, etc.)                |
| **Assembly View** | `.lis` files in `bin/` and `src/` show compiled Z80 ASM for each module                      |

## When Adding New Examples (Modules 3-6)

1. **Create** `src/modules/moduleN.c` with `void run_exampleN(void)` function
2. **Declare** function in `src/modules/modules.h`
3. **Add Case** in `main.c` switch statement routing to new example
4. **Document** in `doc/modulo-N.md` with function links to z88dk-sp1.md
5. **Link** from `doc/Indice.md` central index
6. **Initialize SP1** following the pattern above (mark screen dirty, call `sp1_UpdateNow()` in loop)

## Resources & References

- **z88dk Documentation:** https://github.com/z88dk/z88dk (compiler, libraries, pragmas)
- **SP1 Library:** Differential sprite engine - see `zpragma.inc` stack requirements
- **ZX Spectrum Technical:** 48KB memory model, attribute system, interrupt modes
- **Emulators:** Fuse, ZEsarUX, or online (js99er.net for quick testing)

---

**Generated:** February 2026 | **Last Updated:** For modules 1-2 (initialization, tiles)
