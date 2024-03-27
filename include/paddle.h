#ifndef PADDLE
#define PADDLE

#include <SDL2/SDL.h>

#include "utils.h"

typedef struct
{
    SDL_Rect rect;
} Paddle;

void render_paddle(SDL_Renderer *ren, Paddle *paddle);
void handle_paddle_movement(Paddle *paddle, SDL_Keycode *keycode);

#endif // !PADDLE
