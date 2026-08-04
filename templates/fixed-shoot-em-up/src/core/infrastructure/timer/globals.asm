SECTION bss_user

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; VARIABLES
;
; All timer state lives in BSS (uninitialized RAM). The startup zero-fills
; the entire BSS segment, so leaving these as bare labels is equivalent to
; the previous `db 0` / `dw 0` initialization without polluting ROM.
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

PUBLIC _GLOBAL_TIMER_TICKS
_GLOBAL_TIMER_TICKS:        defs 1    ; ticks within the current second (0..49, resets to 0)

PUBLIC _GLOBAL_TIMER_ABS_TICKS
_GLOBAL_TIMER_ABS_TICKS:    defs 2    ; total ticks since boot (16-bit, never resets)

PUBLIC _GLOBAL_TIMER_PAUSE
_GLOBAL_TIMER_PAUSE:        defs 1    ; pause flag (nonzero = timer frozen)