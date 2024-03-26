CC=gcc
CFLAGS=-Wall -Wextra
INCLUDE=-Iinclude/
LIBS= -lSDL2
SRCS=main.c
OBJS= $(SRCS:.c=.o)
MAIN = breakout

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) -o $(MAIN) $(OBJS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

clean:
	$(RM) *.o *~ $(MAIN)
