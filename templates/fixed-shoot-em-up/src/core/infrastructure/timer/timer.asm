SECTION code_user

INCLUDE "src/core/system_definitions.asm"

PUBLIC _update_timer       ; export C decl "extern void _update_timer(void) __z88dk_fastcall;"

EXTERN _GLOBAL_TIMER_PAUSE
EXTERN _GLOBAL_TIMER_TICKS
EXTERN _GLOBAL_TIMER_ABS_TICKS

;-------------------------------------------------------------------------------
;  Name:		public _update_timer
;  Description:	routine for update timer, should be called from ISR IM2
;  Input:		--
;  Output: 	    --
;-------------------------------------------------------------------------------
_update_timer:

    push af
    push hl

    ld a, (_GLOBAL_TIMER_PAUSE)         ; if is timer paused, exit
    or a
    jr nz, _update_timer_exit

    ld hl, (_GLOBAL_TIMER_ABS_TICKS)    ; increment abs_ticks (absolute ticks)
    inc hl
    ld (_GLOBAL_TIMER_ABS_TICKS), hl

    ld a, (_GLOBAL_TIMER_TICKS)         ; Incrementamos ticks (50/seg)
    inc a
    ld (_GLOBAL_TIMER_TICKS), a

    cp _SYS_TICKS_PER_SECOND            ; ticks < 50,  return, otherwise continue
    jr c, _update_timer_exit

    xor a                               ; ticks = 0
    ld (_GLOBAL_TIMER_TICKS), a

_update_timer_exit:

   pop hl
   pop af
   ret
