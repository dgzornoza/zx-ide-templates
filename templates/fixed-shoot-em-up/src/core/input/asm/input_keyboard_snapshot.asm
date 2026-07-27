SECTION code_user

;-------------------------------------------------------------------------------
; input_keyboard_snapshot.asm
; Captures the ZX Spectrum keyboard matrix state in a single 8-IN sweep of
; port $FE.
;
; Function: extern void input_keyboard_snapshot(void) __z88dk_fastcall;
; Arguments: none
; Description:
;   Writes the inverted (bit=1=key held) state of each keyboard half-row into
;   the global `keyboard_cache[8]` (one byte per half-row, low 5 bits only).
;   The 8 row selectors are $fe, $fd, $fb, $f7, $ef, $df, $bf, $7f (one bit
;   clear per row, the rest set; the cleared bit selects which half-row the
;   port returns).
; Clobbers: AF, BC, DE  (NOT IX/IY, sdcc_iy reserves IY, no PUSH/POP of IX/IY)
; Returns: None
; Stack: unchanged at call site (no args, no saved registers)
;-------------------------------------------------------------------------------
PUBLIC _input_keyboard_snapshot
_input_keyboard_snapshot:
   ld   de, row_table            ; DE -> 8 row selectors
   ld   bc, _keyboard_cache      ; BC -> cache
   ld   l,  8                    ; rows remaining
.rows_loop:
   ld   a, (de)
   in   a, ($fe)                 ; A=row-select at port $(A,$FE); returns active-low 5 keys
   cpl                           ; bit=1 now means pressed
   and  $1f                      ; keep low 5 key bits
   ld   (bc), a
   inc  bc
   inc  de
   dec  l
   jr   nz, rows_loop            ; loop
   ret

.row_table:
   defb $fe, $fd, $fb, $f7, $ef, $df, $bf, $7f

;-------------------------------------------------------------------------------
; BSS storage for the snapshot. C consumers read it via `extern uint8_t keyboard_cache[8];`
; from input_keyboard_snapshot.h. Size matches the 8 half-rows the port sweep populates.
;-------------------------------------------------------------------------------
SECTION bss_user
PUBLIC _keyboard_cache
_keyboard_cache:
   DEFS 8
