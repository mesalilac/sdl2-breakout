#include <SDL2/SDL.h>

#include "macros.h"
#include "paddle.h"

void render_paddle(SDL_Renderer *ren, Paddle *paddle)
{
    SDL_SetRenderDrawColor(ren, HEXCOLOR(PADDLE_COLOR));
    SDL_RenderFillRect(ren, &paddle->rect);
}

void handle_paddle_movement(Paddle *paddle, SDL_Keycode *keycode)
{
    if (*keycode == 'a')
        paddle->rect.x -= 30;
    if (*keycode == 'd')
        paddle->rect.x += 30;
}
