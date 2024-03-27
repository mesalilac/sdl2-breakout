#ifndef GAME
#define GAME

#include <stdbool.h>

#include "ball.h"
#include "brick.h"
#include "paddle.h"

typedef struct
{
    Ball ball;
    Paddle paddle;
    Brick **bricks;
    int brick_count;
    int lifes;
    bool game_over;
} GameState;

void init_game_state(GameState *game_state);
void reset_game_state(GameState *game_state);

#endif // !GAME
