#ifndef __DGZ_TIMER_H__
#define __DGZ_TIMER_H__

/** increments every frame (50 Hz) and rolls over to 0
 * each second. Useful for "which tick of the second" effects (animation
 * phases, blinking, etc.). */
extern volatile unsigned char GLOBAL_TIMER_TICKS;

/** increments every frame and never resets. use it in main loop
 * "wait for next frame" patterns 16-bit width gives
 * ~22 minutes at 50 Hz before wrap. */
extern volatile unsigned int GLOBAL_TIMER_ABS_TICKS;
extern volatile unsigned char GLOBAL_TIMER_PAUSE;

/**
 * Advances the timer state. MUST be called from the IM2 ISR each frame.
 */
extern void update_timer(void) __z88dk_fastcall;

#endif
