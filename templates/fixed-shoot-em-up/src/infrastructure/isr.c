#include <z80.h>
#include <string.h>
#include <stdint.h>
#include <intrinsic.h>
#include <im2.h>

volatile uint8_t frame_tick = 0;

// IM2 uses 0xD0 and 0xD1 because the SP1 library places its data
// structures in the upper RAM area and relocates the stack to 0xD000.
// This guarantees that the IM2 vector table (0xD000–0xD0FF) and the
// jump vector at 0xD1D1 are located in a safe, unused memory region,
// avoiding conflicts with the screen, program code, heap, and SP1
// internal buffers. These addresses are specific to the SP1 memory
// layout and differ from the generic IM2 setup used in non-SP1 programs.
#define TABLE_HIGH_BYTE ((unsigned int)0xD0)
#define JUMP_POINT_HIGH_BYTE ((unsigned int)0xD1)

#define UI_256 ((unsigned int)256)

#define TABLE_ADDR ((void *)(TABLE_HIGH_BYTE * UI_256))
#define JUMP_POINT ((unsigned char *)((unsigned int)(JUMP_POINT_HIGH_BYTE * UI_256) + JUMP_POINT_HIGH_BYTE))

/** IM2 ISR, called every 50 ms (50 Hz vertical blank). */
IM2_DEFINE_ISR(isr)
{
    frame_tick++;
}

/** Installs the IM2 ISR and enables interrupts. */
void im2_setup(void)
{
    memset(TABLE_ADDR, JUMP_POINT_HIGH_BYTE, 257);

    z80_bpoke(JUMP_POINT, 195);
    z80_wpoke(JUMP_POINT + 1, (unsigned int)isr);

    im2_init(TABLE_ADDR);

    intrinsic_ei();
}
