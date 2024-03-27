CC=gcc
CFLAGS=-Wall -Wextra
IDIR=include
INCLUDE=-I$(IDIR)/
LIBS= -lSDL2 -lSDL2_ttf
SRCS=main.c $(IDIR)/game.c $(IDIR)/ball.c $(IDIR)/brick.c $(IDIR)/paddle.c
OBJS= $(SRCS:.c=.o)
MAIN = breakout

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(MAIN) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) *.o include/*.o *~ $(MAIN)
