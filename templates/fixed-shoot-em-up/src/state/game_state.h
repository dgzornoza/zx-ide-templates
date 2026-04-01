#ifndef __STATE_GAME_STATE_H__
#define __STATE_GAME_STATE_H__

#include <stdint.h>

// Define possible game states
typedef enum
{
    STATE_MENU,
    STATE_PLAYING,
    STATE_GAME_OVER
} GameState;

// Global variables exported for the whole game
extern GameState game_state;
extern uint16_t game_score;
extern uint8_t game_lives;

// Initialize state for a new game
void game_state_init(void);

#endif // __STATE_GAME_STATE_H__
