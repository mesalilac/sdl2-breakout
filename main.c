#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WIDTH 800
#define HEIGHT 600

#define BALL_SIZE 15

#define PADDLE_WIDTH 200
#define PADDLE_HEIGHT BALL_SIZE

#define BLOCK_WIDTH 80
#define BLOCK_HEIGHT 30

#define FONT_NAME "./FiraCode-Regular.ttf"

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
    int lifes;
    bool dead;
    SDL_Color color;
} Block;

typedef struct
{
    SDL_Rect rect;
    int speed;
    int xVelocity;
    int yVelocity;
    bool dead;
} Ball;

Ball *ball_new(int x, int y)
{
    Ball *ball = (Ball *)malloc(sizeof(Ball));
    SDL_Rect rect = {
        .x = x,
        .y = y,
        .w = BALL_SIZE,
        .h = BALL_SIZE,
    };
    ball->rect = rect;

    ball->speed = 4;
    ball->xVelocity = 1;
    ball->yVelocity = -1;

    return ball;
}

void ball_update_position(Ball *ball)
{
    if (ball->rect.x < 0)
        ball->xVelocity = 1;
    if (ball->rect.x + ball->rect.w > WIDTH)
        ball->xVelocity = -1;
    if (ball->rect.y < 0)
        ball->yVelocity = 1;
    if (ball->rect.y + ball->rect.h > HEIGHT)
    {
        ball->dead = true;
        return;
    }

    ball->rect.x += ball->xVelocity * ball->speed;
    ball->rect.y += ball->yVelocity * ball->speed;
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

void ball_check_paddle_intersection(Ball *ball, Paddle *paddle)
{
    if (SDL_HasIntersection(&ball->rect, &paddle->rect))
    {
        if (ball->rect.x <= paddle->rect.x)
            ball->xVelocity = -1;
        if (ball->rect.x >= paddle->rect.x + paddle->rect.w)
            ball->xVelocity = 1;
        if (ball->rect.y <= paddle->rect.y)
            ball->yVelocity = -1;
    }
}

void generate_blocks(Block **blocks, int *blocks_count, int row_n, int col_n)
{
    int margin = 60;
    int padding = 20;
    int y = BLOCK_HEIGHT + padding;

    for (int i = 0; i < row_n; ++i)
    {
        for (int j = 0; j < col_n; ++j)
        {
            Block *block = (Block *)malloc(sizeof(Block));
            SDL_Rect rect = {.x = margin + j * (BLOCK_WIDTH + padding), .y = y, .w = BLOCK_WIDTH, .h = BLOCK_HEIGHT};
            block->rect = rect;
            block->dead = false;
            block->lifes = (rand() % (5 - 1)) + 1;

            SDL_Color color = {255, 255, 255, 255};

            switch (block->lifes)
            {
            case 1:
                color.r = 85;
                color.g = 183;
                color.b = 21;
                break;
            case 2:
                color.r = 115;
                color.g = 183;
                color.b = 21;
                break;
            case 3:
                color.r = 183;
                color.g = 182;
                color.b = 21;
                break;
            case 4:
                color.r = 183;
                color.g = 118;
                color.b = 21;
                break;
            case 5:
                color.r = 183;
                color.g = 50;
                color.b = 21;
                break;
            }

            block->color = color;

            blocks[*blocks_count] = block;
            *blocks_count += 1;
        };

        y += BLOCK_HEIGHT + padding;
    }
}

void blocks_render(SDL_Renderer *ren, Block **blocks, int blocks_count)
{

    for (int i = 0; i < blocks_count; ++i)
    {
        Block *block = blocks[i];
        if (block->dead == true)
            continue;

        SDL_SetRenderDrawColor(ren, block->color.r, block->color.g, block->color.b, block->color.a);
        SDL_RenderFillRect(ren, &block->rect);
    }
}

void render_score(SDL_Renderer *ren, int score)
{
    TTF_Font *tip_font = TTF_OpenFont(FONT_NAME, 18);
    SDL_Color color = {255, 255, 255, 255};
    char *text = malloc(50 * sizeof(char));
    sprintf(text, "score: %i", score);

    SDL_Surface *surface = TTF_RenderText_Solid(tip_font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_Rect title_rect = {
        .x = 0,
        .y = HEIGHT - surface->h,
        .w = surface->w,
        .h = surface->h,
    };
    SDL_RenderCopy(ren, texture, NULL, &title_rect);
    SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
    SDL_RenderDrawRect(ren, &title_rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    TTF_CloseFont(tip_font);
}

void render_screen_tip(SDL_Renderer *ren, char *text)
{
    TTF_Font *tip_font = TTF_OpenFont(FONT_NAME, 18);
    SDL_Color color = {255, 255, 255, 255};

    SDL_Surface *surface = TTF_RenderText_Solid(tip_font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_Rect title_rect = {
        .x = WIDTH / 2 - surface->w / 2,
        .y = HEIGHT - 50,
        .w = surface->w,
        .h = surface->h,
    };
    SDL_RenderCopy(ren, texture, NULL, &title_rect);
    SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
    SDL_RenderDrawRect(ren, &title_rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    TTF_CloseFont(tip_font);
}

void render_title(SDL_Renderer *ren, char *text)
{
    TTF_Font *title_font = TTF_OpenFont(FONT_NAME, 28);

    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(title_font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_Rect rect = {
        .x = WIDTH / 2 - surface->w / 2,
        .y = 100,
        .w = surface->w,
        .h = surface->h,
    };
    SDL_RenderCopy(ren, texture, NULL, &rect);
    SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
    SDL_RenderDrawRect(ren, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    TTF_CloseFont(title_font);
}

int main()
{
    srand(time(0));

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr, "ERROR: Failed to init SDL: %s", SDL_GetError());
        exit(1);
    }

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

    if (TTF_Init() < 0)
    {
        fprintf(stderr, "ERROR: Failed to init TTF: %s", SDL_GetError());
        exit(1);
    }

    SDL_Event event;
    bool isRunning = true;
    GameState gameState = PLAYING;
    int score = 0;
    Keys keys = {false};
    int paddleSpeed = 5;
    Paddle paddle = paddle_new();
    Block **blocks = malloc(100 * sizeof(Block));
    int blocks_count = 0;

    Ball *balls[1000];
    int balls_count = 0;

    balls[balls_count] = ball_new(WIDTH / 2 - PADDLE_WIDTH / 2, HEIGHT - PADDLE_HEIGHT - 100);
    ++balls_count;

    generate_blocks(blocks, &blocks_count, 3, 7);

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
                if (event.key.keysym.sym == ' ' && (gameState == GAMEOVER || gameState == WIN))
                {
                    paddle = paddle_new();
                    score = 0;
                    balls_count = 0;
                    balls[balls_count] = ball_new(WIDTH / 2 - PADDLE_WIDTH / 2, HEIGHT - PADDLE_HEIGHT - 100);
                    ++balls_count;
                    gameState = PLAYING;
                }
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

        render_score(ren, score);

        switch (gameState)
        {
        case PLAYING:
            if (keys.left)
                paddle.rect.x -= paddleSpeed;
            if (keys.right)
                paddle.rect.x += paddleSpeed;

            paddle_render(ren, &paddle);

            int dead_balls_count = 0;
            for (int i = 0; i < balls_count; ++i)
            {
                Ball *ball = balls[i];
                if (ball->dead == false)
                {
                    ball_check_paddle_intersection(balls[i], &paddle);
                    ball_update_position(balls[i]);
                    ball_render(ren, balls[i]);

                    for (int j = 0; j < blocks_count; ++j)
                    {
                        Block *block = blocks[j];
                        if (block->dead == true)
                            continue;

                        if (SDL_HasIntersection(&ball->rect, &block->rect))
                        {
                            if (ball->rect.x > block->rect.x + block->rect.w)
                                ball->xVelocity = 1;
                            if (ball->rect.x + ball->rect.w < block->rect.x)
                                ball->xVelocity = -1;

                            if (ball->rect.y > block->rect.y)
                                ball->yVelocity = 1;
                            if (ball->rect.y < block->rect.y)
                                ball->yVelocity = -1;

                            block->lifes -= 1;
                            if (block->lifes == 0)
                                block->dead = true;
                        }
                    }
                }
                else if (ball->dead == true)
                {
                    ++dead_balls_count;
                }
            }
            if (dead_balls_count == balls_count)
                gameState = GAMEOVER;

            blocks_render(ren, blocks, blocks_count);
            break;
        case GAMEOVER:
            render_title(ren, "GAMEOVER");
            render_screen_tip(ren, "Press the space bar to restart the game!");
            break;
        case WIN:
            render_title(ren, "YOU WON");
            render_screen_tip(ren, "Press the space bar to restart the game!");
            break;
        }

        SDL_RenderPresent(ren);
    }

    SDL_DestroyWindow(win);
    SDL_DestroyRenderer(ren);
    SDL_Quit();
}
