#include <SDL2/SDL.h>
#include <stdbool.h>

#include "ball.h"
#include "macros.h"
#include "paddle.h"

void render_ball(SDL_Renderer *ren, Ball *ball)
{
    SDL_SetRenderDrawColor(ren, HEXCOLOR(BALL_COLOR));
    SDL_RenderFillRect(ren, &ball->rect);
}

// Move the ball
void handle_ball_movement(Ball *ball, int *lifes, bool *game_over)
{
    if (ball->rect.x < 0)
        ball->velocity.x = 1;
    else if (ball->rect.y < 0)
        ball->velocity.y = 1;

    if (ball->rect.x + ball->rect.w > WINDOW_WIDTH)
        ball->velocity.x = -1;
    else if (ball->rect.y + ball->rect.h > WINDOW_HEIGHT)
    {
        *lifes -= 1;
        if (*lifes == 0)
            *game_over = true;

        ball->rect.x = WINDOW_WIDTH / 2;
        ball->rect.y = WINDOW_HEIGHT / 2;
    }

    ball->rect.x += ball->velocity.x * SPEED;
    ball->rect.y += ball->velocity.y * SPEED;
}

void handle_paddle_collision(Ball *ball, Paddle *paddle)
{
    if (SDL_HasIntersection(&ball->rect, &paddle->rect))
    {
        if (ball->rect.y <= paddle->rect.y)
            ball->velocity.y = -1;
        if (ball->rect.x + ball->rect.w <= paddle->rect.x)
            ball->velocity.x = -1;
        if (ball->rect.x >= paddle->rect.x + paddle->rect.w)
            ball->velocity.x = 1;
        if (ball->rect.y >= paddle->rect.y)
            ball->velocity.y = 1;
    }
}
