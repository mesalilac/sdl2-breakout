#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define WIDTH 800
#define HEIGHT 600

typedef enum
{
    PLAYING,
    WIN,
    GAMEOVER,
} GameState;

typedef struct
{
    SDL_Rect rect;
} Player;

Player player_new()
{
    Player player = {.rect = {.x = 100, .y = 100, .w = 30, .h = 30}};

    return player;
}

void player_render(SDL_Renderer *ren, Player *player)
{
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderFillRect(ren, &player->rect);
}

int main()
{
    SDL_Window *win = SDL_CreateWindow("Breakout", 0, 0, WIDTH, HEIGHT, 0);
    if (win == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create window: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (ren == NULL)
    {
        fprintf(stderr, "ERROR: Failed to create renderer: %s", SDL_GetError());
        exit(1);
    }

    SDL_Event event;
    bool isRunning = true;
    GameState gameState = PLAYING;
    Player player = player_new();

    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;
            }
        }

        SDL_SetRenderDrawColor(ren, 18, 18, 18, 255);
        SDL_RenderClear(ren);

        switch (gameState)
        {
        case PLAYING:
            player_render(ren, &player);
            break;
        case GAMEOVER:
            break;
        case WIN:
            break;
        }

        SDL_RenderPresent(ren);
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
