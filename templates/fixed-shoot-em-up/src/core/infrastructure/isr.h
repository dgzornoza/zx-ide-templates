#ifndef __CORE_INFRASTRUCTURE_ISR_H__
#define __CORE_INFRASTRUCTURE_ISR_H__

#include <stdint.h>

extern volatile uint8_t frame_tick;

/** Initializes the IM2 interrupt vector table and installs the ISR. */
void im2_setup(void);

#endif // __CORE_INFRASTRUCTURE_ISR_H__
