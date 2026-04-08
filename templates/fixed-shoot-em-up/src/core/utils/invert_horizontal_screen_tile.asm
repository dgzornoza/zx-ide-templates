;-------------------------------------------------------------------------------
; invert_horizontal_screen_char.asm
; Optimized function to invert a ZX Spectrum 8x8 screen character horizontally in-place using a lookup table.
;
; Function: void __z88dk_fastcall _invert_horizontal_screen_char(void *address)
; Arguments:
;   HL = pointer to screen character (8 bytes, one per row), should point to the first scanline of the character
; Description:
;   For each row of the character, replaces the byte with its horizontally mirrored value
;   using a precomputed mirror table. The operation is performed in-place.
; Clobbers: AF, BC, DE, HL
; Returns: None
;-------------------------------------------------------------------------------
SECTION code_user

PUBLIC _invert_horizontal_screen_char

_invert_horizontal_screen_char:
    ld d, _mirror_table >> 8      ; D = high byte of mirror table (table must be aligned) (ei: $68 if is $6800)
    ld b, 8                       ; B = 8 scanlines/rows to process
.loop:
    ld a, (hl)                    ; Load original byte (row)
    ld e, a                       ; Use as low byte for table pointer (DE = mirror_table + value)
    ld a, (de)                    ; Load mirrored value from table

    ld (hl), a                    ; Store mirrored value back to character
    inc h                         ; Jump to next line of the character
                                  ; In the Spectrum, adding 1 to H jumps to the next
                                  ; scanline of the same character position.
    djnz loop                     ; Repeat for all 8 scanlines/rows

    ret


; Table of mirrored byte values for all 256 possible byte inputs (0-255). Each entry is the horizontal mirror of the index.
; The table is aligned to 256 bytes to allow for efficient indexing using the input byte as the low byte of the address.
SECTION bss_user
ALIGN 256             ; It is vital that the table is aligned to 256 bytes for use optimized inversion function.
_mirror_table:
    defb $00,$80,$40,$C0,$20,$A0,$60,$E0,$10,$90,$50,$D0,$30,$B0,$70,$F0
    defb $08,$88,$48,$C8,$28,$A8,$68,$E8,$18,$98,$58,$D8,$38,$B8,$78,$F8
    defb $04,$84,$44,$C4,$24,$A4,$64,$E4,$14,$94,$54,$D4,$34,$B4,$74,$F4
    defb $0C,$8C,$4C,$CC,$2C,$AC,$6C,$EC,$1C,$9C,$5C,$DC,$3C,$BC,$7C,$FC
    defb $02,$82,$42,$C2,$22,$A2,$62,$E2,$12,$92,$52,$D2,$32,$B2,$72,$F2
    defb $0A,$8A,$4A,$CA,$2A,$AA,$6A,$EA,$1A,$9A,$5A,$DA,$3A,$BA,$7A,$FA
    defb $06,$86,$46,$C6,$26,$A6,$66,$E6,$16,$96,$56,$D6,$36,$B6,$76,$F6
    defb $0E,$8E,$4E,$CE,$2E,$AE,$6E,$EE,$1E,$9E,$5E,$DE,$3E,$BE,$7E,$FE
    defb $01,$81,$41,$C1,$21,$A1,$61,$E1,$11,$91,$51,$D1,$31,$B1,$71,$F1
    defb $09,$89,$49,$C9,$29,$A9,$69,$E9,$19,$99,$59,$D9,$39,$B9,$79,$F9
    defb $05,$85,$45,$C5,$25,$A5,$65,$E5,$15,$95,$55,$D5,$35,$B5,$75,$F5
    defb $0D,$8D,$4D,$CD,$2D,$AD,$6D,$ED,$1D,$9D,$5D,$DD,$3D,$BD,$7D,$FD
    defb $03,$83,$43,$C3,$23,$A3,$63,$E3,$13,$93,$53,$D3,$33,$B3,$73,$F3
    defb $0B,$8B,$4B,$CB,$2B,$AB,$6B,$EB,$1B,$9B,$5B,$DB,$3B,$BB,$7B,$FB
    defb $07,$87,$47,$C7,$27,$A7,$67,$E7,$17,$97,$57,$D7,$37,$B7,$77,$F7
    defb $0F,$8F,$4F,$CF,$2F,$AF,$6F,$EF,$1F,$9F,$5F,$DF,$3F,$BF,$7F,$FF
