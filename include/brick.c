#include <SDL2/SDL.h>

#include "ball.h"
#include "brick.h"
#include "macros.h"

void render_bricks(SDL_Renderer *ren, Brick **bricks, int count)
{
    for (int i = 0; i < count; ++i)
    {
        Brick *brick = bricks[i];

        if (!brick->is_dead)
        {

            SDL_SetRenderDrawColor(ren, HEXCOLOR(brick->color));
            SDL_RenderFillRect(ren, &brick->rect);
        }
    }
}

void handle_bricks_collision(Ball *ball, Brick **bricks, int count)
{
    for (int i = 0; i < count; ++i)
    {
        Brick *brick = bricks[i];

        if (SDL_HasIntersection(&ball->rect, &brick->rect) && !brick->is_dead)
        {
            if (ball->rect.y <= brick->rect.y)
                ball->velocity.y = -1;
            if (ball->rect.x + ball->rect.w <= brick->rect.x)
                ball->velocity.x = -1;
            if (ball->rect.x >= brick->rect.x + brick->rect.w)
                ball->velocity.x = 1;
            if (ball->rect.y >= brick->rect.y)
                ball->velocity.y = 1;

            brick->is_dead = true;
        }
    }
}
