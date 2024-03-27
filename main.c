#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define HEXCOLOR(code)                                                                                                 \
    ((code) >> (3 * 8)) & 0xFF, ((code) >> (2 * 8)) & 0xFF, ((code) >> (1 * 8)) & 0xFF, ((code) >> (0 * 8)) & 0xFF

#define BACKGROUND_COLOR 0x1b1b1b

int main()
{
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
            }
        }

        SDL_SetRenderDrawColor(ren, HEXCOLOR(BACKGROUND_COLOR));
        SDL_RenderClear(ren);

        SDL_RenderPresent(ren);
    }

    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
