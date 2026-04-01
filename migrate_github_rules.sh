#!/bin/bash
set -e

mkdir -p /workspaces/zx-ide-templates/.github/instructions
mkdir -p /workspaces/zx-ide-templates/.github/skills/create-entity

# Move instructions and skills
mv /workspaces/zx-ide-templates/templates/fixed-shoot-em-up/.github/instructions/* /workspaces/zx-ide-templates/.github/instructions/ || true
mv /workspaces/zx-ide-templates/templates/fixed-shoot-em-up/.github/skills/create-entity/SKILL.md /workspaces/zx-ide-templates/.github/skills/create-entity/SKILL.md || true

# Remove old .github folder in the template
rm -rf /workspaces/zx-ide-templates/templates/fixed-shoot-em-up/.github

# Write the new merged copilot-instructions.md
cat << 'INNER_EOF' > /workspaces/zx-ide-templates/.github/copilot-instructions.md
# AI Coding Instructions for zx-ide-templates

## Project Overview

This workspace contains **ZX Spectrum game development templates** using the z88dk compiler and SP1 graphics engine.

- **Target:** ZX Spectrum (8-bit retro platform)
- **Language:** C + Z80 Assembly (via z88dk/SDCC)
- **Graphics Engine:** SP1 (differential update sprite engine)
- **Build System:** Makefile-based (z88dk compiler toolchain)
- **Reference docs:** `docs/z88dk-sp1.md`

## Core Language Rules

- **English Only**: ALL variables, function/method names, and code comments MUST ALWAYS be written in English.

## Global Z88DK/Z80 & SP1 Rules (Instructions)

The following guidelines dictate the architecture of the game. They are loaded automatically by Copilot when relevant:

- **[Z80 C Architecture Guidelines](./instructions/z80-c-architecture.instructions.md)** (Global Z88DK/Z80): Details the strict rules for state management, memory allocation, and function arguments to optimize code for the Z80 CPU.
- **[SP1 Rendering & Game Loop](./instructions/sp1-rendering.instructions.md)** (SP1 Specific): Rules for the 50Hz game loop, IM2 interrupts, CPU yielding, and efficient conditional rendering.

## Agent Skills

- **`/create-entity`**: Use the **[Game Entity Creation Skill](./skills/create-entity/SKILL.md)** whenever you need to create a new game element (e.g., player, enemy, item, layout). It provides a strict workflow to enforce the zero-parameter and zero-allocation hardware rules.

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
INNER_EOF

echo "Migration complete!"
