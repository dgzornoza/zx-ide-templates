;-------------------------------------------------------------------------------
; system_definitions.asm
;
; Assembly twin of system_definitions.h.
;
; Synchronization contract:
;   Every EQU here MUST match the corresponding #define in
;   system_definitions.h. If you change a value, change BOTH files. The
;   preprocessor will not catch a mismatch.
;
; Scope rule (per project-architecture §5):
;   This file MUST contain ONLY EQU declarations and comments. No
;   executable code, no initialized data (defb/defw), no SECTION, no
;   PUBLIC/EXTERN bookkeeping. Symbols are local to each including file.
;-------------------------------------------------------------------------------

; --- System tick rate (50 Hz on ZX Spectrum 48K, vertical blank) ---
_SYS_TICKS_PER_SECOND EQU 50

; --- Screen dimensions in pixels ---
_SYS_SCREEN_WIDTH  EQU 256          ; 256 does not fit in 8-bit; use as 16-bit immediate
_SYS_SCREEN_HEIGHT EQU 192

; --- Single character cell size in pixels ---
_SYS_CHAR_SIZE     EQU 8

; --- Screen dimensions in character cells (precomputed: pixels / char_size) ---
_SYS_SCREEN_CHARS_WIDTH        EQU 32   ; 256 / 8
_SYS_SCREEN_CHARS_WIDTH_BASE_0 EQU 31   ; 256 / 8 - 1
_SYS_SCREEN_CHARS_HEIGHT       EQU 24   ; 192 / 8
_SYS_SCREEN_CHARS_HEIGHT_BASE_0 EQU 23  ; 192 / 8 - 1
