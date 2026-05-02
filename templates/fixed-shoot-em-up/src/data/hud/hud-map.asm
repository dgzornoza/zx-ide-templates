; HUD_MAP_WIDTH: 16
; HUD_MAP_HEIGHT: 4
; HUD_MAP_SIZE: 64
; HUD_MAP_TILES_COUNT: 64
; Data Size: 64 bytes
SECTION rodata_user
PUBLIC _hud_map

; Map: hud-map  (16 x 4 tiles)
_hud_map:
    defb 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16
    defb 17,18,19,20,21,0,0,0,0,0,22,23,24,25,26,27
    defb 28,29,30,31,0,0,0,0,0,0,0,33,34,35,36,37
    defb 38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53
