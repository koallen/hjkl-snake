# compiler
CC = gcc

# flags
LDFLAGS = -lncurses
CCFLAGS = -Wall

# target rules
all: snake

snake: snake.o
	$(CC) -o $@ $? $(LDFLAGS)

%.o: %.c
	$(CC) -c $< $(CCFLAGS)

clean:
	rm -f *.o snake
