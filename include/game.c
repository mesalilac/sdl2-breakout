#include <SDL2/SDL.h>

#include "brick.h"
#include "game.h"
#include "macros.h"

#define RAINBOW_COLORS_COUNT 7
const int rainbow_colors[RAINBOW_COLORS_COUNT] = {0xFF0000FF, 0xFF7F00FF, 0xFFFF00FF, 0x00FF00FF,
                                                  0x0000FFFF, 0x4B0082FF, 0x9400D3FF};

void init_game_state(GameState *game_state)
{
    SDL_Rect ball_rect = {.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2, .w = BALL_SIZE, .h = BALL_SIZE};
    game_state->ball.rect = ball_rect;
    Point point = {.x = -1, .y = -1};
    game_state->ball.velocity = point;

    SDL_Rect paddle_rect = {
        .x = WINDOW_WIDTH / 2 - PADDLE_WIDTH / 2, .y = WINDOW_HEIGHT - 100, .w = PADDLE_WIDTH, .h = PADDLE_HEIGHT};
    game_state->paddle.rect = paddle_rect;

    game_state->bricks = (Brick **)malloc(BRICKS_COUNT * sizeof(Brick));
    game_state->brick_count = 0;

    // Generate bricks
    {
        int x = 0;
        int y = 0;
        int row = 0;

        for (int i = 0; i < BRICKS_COUNT; ++i)
        {
            // TODO: Cut brick to fix if width is greater of max

            Brick *brick = (Brick *)malloc(sizeof(Brick));
            SDL_Rect rect = {.x = x, .y = y, .w = BRICK_WIDTH, .h = BRICK_HEIGHT};

            brick->rect = rect;
            brick->is_dead = false;
            brick->color = rainbow_colors[row % RAINBOW_COLORS_COUNT];
            game_state->bricks[game_state->brick_count] = brick;

            game_state->brick_count += 1;
            x += BRICK_WIDTH;
            if (x > WINDOW_WIDTH)
            {
                x = 0;
                y += BRICK_HEIGHT;
                row++;
            }
        }
    }

    game_state->lifes = 3;
    game_state->game_over = false;
}
