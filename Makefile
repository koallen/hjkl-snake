# compiler
CC = gcc

# flags
LDFLAGS = -lncurses
CFLAGS = -std=c99 -Wall

# target rules
all: snake

snake: snake.o
	$(CC) -o $@ $? $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

.PHONY: clean
clean:
	rm -f *.o snake
