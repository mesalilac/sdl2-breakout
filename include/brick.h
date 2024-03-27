#ifndef BRICK
#define BRICK

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "ball.h"

typedef struct
{
    SDL_Rect rect;
    bool is_dead;
    int color;
} Brick;

void render_bricks(SDL_Renderer *ren, Brick **bricks, int count);
void handle_bricks_collision(Ball *ball, Brick **bricks, int count);

#endif // !BRICK
