#ifndef BALL
#define BALL

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "paddle.h"
#include "utils.h"

typedef struct
{
    SDL_Rect rect;
    Point velocity;
} Ball;

void render_ball(SDL_Renderer *ren, Ball *ball);
void handle_ball_movement(Ball *ball, int *lifes, bool *game_over);
void handle_paddle_collision(Ball *ball, Paddle *paddle);

#endif // !BALL
