#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "include/ball.h"
#include "include/brick.h"
#include "include/game.h"
#include "include/macros.h"
#include "include/paddle.h"

/*
 TODO: Win Screen
*/

void draw_lifes(SDL_Renderer *ren, TTF_Font *font, int lifes)
{
    SDL_Color font_color = {255, 255, 255, 255};

    char *text = malloc(50 * sizeof(char));
    sprintf(text, "lifes %i", lifes);
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, font_color);
    SDL_Texture *message = SDL_CreateTextureFromSurface(ren, surface);
    SDL_Rect message_rect = {.x = 0, .y = WINDOW_HEIGHT - surface->h, .w = surface->w, .h = surface->h};

    SDL_RenderCopy(ren, message, NULL, &message_rect);
    SDL_SetRenderDrawColor(ren, HEXCOLOR(BACKGROUND_COLOR));
    SDL_RenderDrawRect(ren, &message_rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(message);
    free(text);
}

int main()
{
    srand(time(0));

    if (SDL_Init(SDL_INIT_VIDEO) == -1)
    {
        fprintf(stderr, "ERROR: Failed to initalize SDL_INIT_VIDEO: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Window *win = SDL_CreateWindow("Breakout", 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (win == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create window: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create renderer: %s\n", SDL_GetError());
        exit(1);
    }

    if (TTF_Init() == -1)
    {
        fprintf(stderr, "ERROR: Failed to initalize TTF: %s\n", SDL_GetError());
        exit(1);
    }

    TTF_Font *font = TTF_OpenFont("FiraCode-Regular.ttf", 18);
    SDL_Color font_color = {255, 255, 255, 255};

    GameState game_state = {0};
    init_game_state(&game_state);

    bool quit = false;
    SDL_Event event;

    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_KEYDOWN:
                // TODO: Make paddle movement smoother
                handle_paddle_movement(&game_state.paddle, &event.key.keysym.sym);
                if (game_state.game_over == true && event.key.keysym.sym == ' ')
                    init_game_state(&game_state); // reset game state
                break;
            }
        }

        SDL_SetRenderDrawColor(ren, HEXCOLOR(BACKGROUND_COLOR));
        SDL_RenderClear(ren);

        if (!game_state.game_over)
        {
            handle_ball_movement(&game_state.ball, &game_state.lifes, &game_state.game_over);
            handle_paddle_collision(&game_state.ball, &game_state.paddle);
            handle_bricks_collision(&game_state.ball, game_state.bricks, game_state.brick_count);

            render_ball(ren, &game_state.ball);
            render_paddle(ren, &game_state.paddle);
            render_bricks(ren, game_state.bricks, game_state.brick_count);

            draw_lifes(ren, font, game_state.lifes);
        }
        else
        {
            {
                SDL_Surface *surface = TTF_RenderText_Solid(font, "GAME OVER", font_color);
                SDL_Texture *message = SDL_CreateTextureFromSurface(ren, surface);
                SDL_Rect message_rect = {
                    .x = WINDOW_WIDTH / 2 - surface->w / 2, .y = WINDOW_HEIGHT / 2.5, .w = surface->w, .h = surface->h};

                SDL_RenderCopy(ren, message, NULL, &message_rect);
                SDL_SetRenderDrawColor(ren, HEXCOLOR(BACKGROUND_COLOR));
                SDL_RenderDrawRect(ren, &message_rect);

                SDL_FreeSurface(surface);
                SDL_DestroyTexture(message);
            }
            {
                SDL_Surface *surface = TTF_RenderText_Solid(font, "press SPACE to play again.", font_color);
                SDL_Texture *message = SDL_CreateTextureFromSurface(ren, surface);
                SDL_Rect message_rect = {
                    .x = WINDOW_WIDTH / 2 - surface->w / 2, .y = WINDOW_HEIGHT - 100, .w = surface->w, .h = surface->h};

                SDL_RenderCopy(ren, message, NULL, &message_rect);
                SDL_SetRenderDrawColor(ren, HEXCOLOR(BACKGROUND_COLOR));
                SDL_RenderDrawRect(ren, &message_rect);

                SDL_FreeSurface(surface);
                SDL_DestroyTexture(message);
            }
        }

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
