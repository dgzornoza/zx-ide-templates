---
name: z88dk-im2-setup
description: "Use when creating or refactoring IM2 interrupt setup on ZX Spectrum with z88dk, including SP1 and non-SP1 memory layouts."
user-invocable: true
argument-hint: "Describe whether you are using SP1, your memory map constraints, and where you want im2_setup/isr files."
---

# z88dk IM2 Setup Patterns

## When to Use

- Add IM2 interrupt support to a new template.
- Refactor existing ISR code into `src/infrastructure/`.
- Migrate between SP1 and non-SP1 projects.
- Validate IM2 page selection against your memory map.

## Pattern A: IM2 with SP1

Use this when SP1 is active and stack relocation is configured with `REGISTER_SP = 0xd000`.

- Preferred high bytes: `0xD0` for table page and `0xD1` for jump point page.
- Reason: SP1 stack relocation makes `0xD000-0xD1FF` a safe and predictable region.

```c
#define TABLE_HIGH_BYTE      ((unsigned int)0xD0)
#define JUMP_POINT_HIGH_BYTE ((unsigned int)0xD1)
#define TABLE_ADDR  ((void *)(TABLE_HIGH_BYTE * 256u))
#define JUMP_POINT  ((unsigned char *)((unsigned int)(JUMP_POINT_HIGH_BYTE * 256u) + JUMP_POINT_HIGH_BYTE))

IM2_DEFINE_ISR(isr) {
    /* 50 Hz game tick logic */
}

void im2_setup(void) {
    memset(TABLE_ADDR, JUMP_POINT_HIGH_BYTE, 257);
    z80_bpoke(JUMP_POINT, 195);
    z80_wpoke(JUMP_POINT + 1, (unsigned int)isr);
    im2_init(TABLE_ADDR);
    intrinsic_ei();
}
```

## Pattern B: IM2 without SP1 (General z88dk)

Use this when SP1 is not used and memory layout is defined by your own linker/org settings.

- Choose an aligned and safe page (for example `0xFE`) that does not collide with stack, buffers, or runtime areas.
- It is valid to use the same page for table and jump point if your map allows it.

```c
#define TABLE_HIGH_BYTE      ((unsigned int)0xFE)
#define JUMP_POINT_HIGH_BYTE ((unsigned int)0xFE)
#define TABLE_ADDR  ((void *)(TABLE_HIGH_BYTE * 256u))
#define JUMP_POINT  ((unsigned char *)((unsigned int)(JUMP_POINT_HIGH_BYTE * 256u) + JUMP_POINT_HIGH_BYTE))

IM2_DEFINE_ISR(isr) {
    /* custom interrupt logic */
}

void im2_setup(void) {
    memset(TABLE_ADDR, JUMP_POINT_HIGH_BYTE, 257);
    z80_bpoke(JUMP_POINT, 195);
    z80_wpoke(JUMP_POINT + 1, (unsigned int)isr);
    im2_init(TABLE_ADDR);
    intrinsic_ei();
}
```

## Placement Rules

- Keep ISR and setup code in `src/infrastructure/isr.c` and expose only `im2_setup(void)` in `src/infrastructure/isr.h`.
- Keep `main.c` free of IM2 implementation details.
- Keep ISR logic short and deterministic.

## Output Contract

When this skill is applied, provide:

1. The selected IM2 page strategy and why.
2. The target files to modify/create.
3. The exact public interface (`im2_setup(void)`) used by `main.c`.
