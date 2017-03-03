# compiler
CC = gcc

# flags
LDFLAGS = -lncurses

# target rules
all: snake

snake: snake.o
	$(CC) -o $@ $? $(LDFLAGS)

%.o: %.c
	$(CC) -c $<
