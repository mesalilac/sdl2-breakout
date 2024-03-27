#ifndef MACROS
#define MACROS

#define HEXCOLOR(code)                                                                                                 \
    ((code) >> (3 * 8)) & 0xFF, ((code) >> (2 * 8)) & 0xFF, ((code) >> (1 * 8)) & 0xFF, ((code) >> (0 * 8)) & 0xFF

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

#define SPEED 5

#define BACKGROUND_COLOR 0x23282dFF

#define BALL_SIZE 30
#define BALL_COLOR 0xEE0000FF

#define PADDLE_WIDTH 150
#define PADDLE_HEIGHT 20
#define PADDLE_COLOR 0xFFFFFFFF

#define BRICK_WIDTH 120
#define BRICK_HEIGHT 10
#define BRICKS_COUNT 50
#define BRICK_COLOR 0xEEFFAAFF

#endif // !MACROS
