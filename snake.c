#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define DELAY 130000

typedef struct {
	int x;
	int y;
} Direction;

typedef struct {
	int x;
	int y;
} Point;

typedef struct {
	Point location;
	bool available;
} Food;

typedef struct {
	Point *body;
	int length;
	Direction direction;
	bool dead;
} Snake;

typedef struct {
	int x;
	int y;
} WindowSize;

void MoveSnake(WindowSize, Snake *, Food *);
void GenerateFood(WindowSize, Snake, Food *);
void UpdateDirection(char, Snake *);

int main()
{
	/*
	 * initialize ncurses screen
	 */
	initscr();
	noecho();
	curs_set(FALSE);
	nodelay(stdscr, TRUE); // don't block on getch() call
	WindowSize window_size = {0, 0};
	getmaxyx(stdscr, window_size.y, window_size.x); // get window size

	/*
	 * initialize game
	 */
	srand(time(NULL));

	Snake snake = {(Point *)malloc(sizeof(Point)), 1, {0, 0}, false};
	snake.body[0].x = 0;
	snake.body[0].y = 0;

	Food food = {{0, 0}, false};

	/*
	 * main loop
	 */
	mvprintw(window_size.y, 0, "Quit the game with 'q'");
	window_size.y--; // last line reserved for the above instruction
	mvprintw(snake.body[0].y, snake.body[0].x, "o");
	GenerateFood(window_size, snake, &food);
	mvprintw(food.location.y, food.location.x, "=");

	refresh();

	while (1)
	{
		char key_stroke = getch();
		switch (key_stroke)
		{
			case 'q':
				goto END;
			default:
				UpdateDirection(key_stroke, &snake);
		}

		clear();

		mvprintw(window_size.y, 0, "Quit the game with 'q'");
		MoveSnake(window_size, &snake, &food);

		if (snake.dead)
			break;

		if (!food.available)
			GenerateFood(window_size, snake, &food);
		mvprintw(food.location.y, food.location.x, "=");

		refresh();
		usleep(DELAY);
	}

	/*
	 * finalize ncurses screen
	 */
END:
	endwin();

	return EXIT_SUCCESS;
}

void MoveSnake(WindowSize window_size, Snake *snake, Food *food)
{
	int old_head_x = snake->body[0].x, old_head_y = snake->body[0].y;
	int new_head_x, new_head_y;
	new_head_x = old_head_x + snake->direction.x;
	new_head_y = old_head_y + snake->direction.y;

	// detect collision
	if (new_head_x < 0 || new_head_x > window_size.x || new_head_y < 0 || new_head_y > window_size.y)
	{
		snake->dead = true;
		return;
	}
	for (int i = 1; i < snake->length; ++i)
		if (new_head_x == snake->body[i].x && new_head_y == snake->body[i].y)
		{
			snake->dead = true;
			return;
		}

	// detect whether food is eaten by next move
	if (new_head_x == food->location.x && new_head_y == food->location.y)
	{
		food->available = false;
		snake->length++;
		snake->body = realloc(snake->body, snake->length * sizeof(Point)); // extend the snake by 1
	}

	// move snake
	for (int i = snake->length - 1; i > 0; --i)
	{
		snake->body[i].x = snake->body[i-1].x;
		snake->body[i].y = snake->body[i-1].y;
	}
	snake->body[0].x = new_head_x;
	snake->body[0].y = new_head_y;

	for (int i = 0; i < snake->length; ++i)
		mvprintw(snake->body[i].y, snake->body[i].x, "o");
}

void GenerateFood(WindowSize window_size, Snake snake, Food *food)
{
	bool food_on_snake = true;

	// generate a food which is not on the body of snake
	while (food_on_snake)
	{
		food->location.x = rand() % window_size.x;
		food->location.y = rand() % window_size.y;
		for (int i = 0; i < snake.length; ++i)
			if (food->location.x == snake.body[i].x && food->location.y == snake.body[i].y)
				food_on_snake = true;
		food_on_snake = false;
	}

	food->available = true;
}

void UpdateDirection(char key_stroke, Snake *snake)
{
	switch(key_stroke)
	{
		case 'h':
			snake->direction.x = -1;
			snake->direction.y = 0;
			break;
		case 'j':
			snake->direction.x = 0;
			snake->direction.y = 1;
			break;
		case 'k':
			snake->direction.x = 0;
			snake->direction.y = -1;
			break;
		case 'l':
			snake->direction.x = 1;
			snake->direction.y = 0;
			break;
	}	
}
