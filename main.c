#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600

#define BALL_SIZE 30

#define PADDLE_WIDTH 200
#define PADDLE_HEIGHT BALL_SIZE

typedef enum
{
    PLAYING,
    WIN,
    GAMEOVER,
} GameState;

typedef struct
{
    bool right;
    bool left;
} Keys;

typedef struct
{
    SDL_Rect rect;
    int xVelocity;
    int yVelocity;
} Ball;

Ball ball_new()
{
    Ball ball = {.rect =
                     {
                         .x = WIDTH / 2 - PADDLE_WIDTH / 2,
                         .y = HEIGHT - PADDLE_HEIGHT - 100,
                         .w = BALL_SIZE,
                         .h = BALL_SIZE,
                     },
                 .xVelocity = 1,
                 .yVelocity = -1};

    return ball;
}

void ball_render(SDL_Renderer *ren, Ball *ball)
{
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderFillRect(ren, &ball->rect);
}

typedef struct
{
    SDL_Rect rect;
} Paddle;

Paddle paddle_new()
{
    Paddle paddle = {.rect = {.x = WIDTH / 2 - PADDLE_WIDTH / 2,
                              .y = HEIGHT - PADDLE_HEIGHT - 50,
                              .w = PADDLE_WIDTH,
                              .h = PADDLE_HEIGHT}};

    return paddle;
}

void paddle_render(SDL_Renderer *ren, Paddle *paddle)
{
    SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
    SDL_RenderFillRect(ren, &paddle->rect);
}

int main()
{
    srand(time(0));

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
    int score = 0;
    Keys keys = {false};
    int paddleSpeed = 5;
    int ballSpeed = 4;
    Paddle paddle = paddle_new();
    Ball ball = ball_new();

    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == 'a')
                    keys.left = true;
                if (event.key.keysym.sym == 'd')
                    keys.right = true;
                break;
            case SDL_KEYUP:
                if (event.key.keysym.sym == 'a')
                    keys.left = false;
                if (event.key.keysym.sym == 'd')
                    keys.right = false;
                break;
            }
        }

        SDL_SetRenderDrawColor(ren, 18, 18, 18, 255);
        SDL_RenderClear(ren);

        switch (gameState)
        {
        case PLAYING:
            if (keys.left)
                paddle.rect.x -= paddleSpeed;
            if (keys.right)
                paddle.rect.x += paddleSpeed;

            if (ball.rect.x < 0)
                ball.xVelocity = 1;
            if (ball.rect.x + ball.rect.w > WIDTH)
                ball.xVelocity = -1;
            if (ball.rect.y < 0)
                ball.yVelocity = 1;
            if (ball.rect.y + ball.rect.h > HEIGHT)
                ball.yVelocity = -1;

            ball.rect.x += ball.xVelocity * ballSpeed;
            ball.rect.y += ball.yVelocity * ballSpeed;

            paddle_render(ren, &paddle);
            ball_render(ren, &ball);
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
