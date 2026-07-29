#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#include <stdint.h>

// Define possible game states
typedef enum
{
    STATE_SPLASH,
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER
} GameState;

// Global variables exported for the whole game
extern GameState game_state;
extern uint16_t game_score;
extern uint8_t game_lives;

#endif // __GAME_STATE_H__
