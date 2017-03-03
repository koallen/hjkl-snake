#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define DELAY 140000

typedef struct {
	int x_direction;
	int y_direction;
} Direction;

typedef struct {
	int x;
	int y;
} Point;

void MoveSnake(Point *snake, Direction direction, int *snake_length, Point food, bool *food_available, int, int, bool *);
void GenerateFood(Point *snake, int snake_length, int max_x, int max_y, Point *food, bool *);

int main()
{
	/*
	 * initialize ncurses screen
	 */
	srand(time(NULL));
	initscr();
	noecho();
	curs_set(FALSE);
	nodelay(stdscr, TRUE); // don't block on getch() call
	int max_x = 0, max_y = 0;
	getmaxyx(stdscr, max_y, max_x); // get window size
	Point *snake = (Point *)malloc(sizeof(Point));
	int snake_length = 1;
	snake[0].x = 0;
	snake[0].y = 0;
	Direction direction = {0, 0};
	Point food;
	bool dead = false;

	/*
	 * main loop
	 */
	mvprintw(max_y, 0, "Quit the game with 'q'");
	max_y--;
	mvprintw(snake[0].y, snake[0].x, "o");
	bool food_available = false;
	GenerateFood(snake, snake_length, max_x, max_y, &food, &food_available);
	refresh();
	while (1)
	{
		char key_stroke = getch();
		switch (key_stroke)
		{
			case 'h':
				direction.x_direction = -1;
				direction.y_direction = 0;
				break;
			case 'j':
				direction.x_direction = 0;
				direction.y_direction = 1;
				break;
			case 'k':
				direction.x_direction = 0;
				direction.y_direction = -1;
				break;
			case 'l':
				direction.x_direction = 1;
				direction.y_direction = 0;
				break;
			case 'q':
				goto END;
				break;
		}
		clear();
		mvprintw(max_y, 0, "Quit the game with 'q'");
		MoveSnake(snake, direction, &snake_length, food, &food_available, max_x, max_y, &dead);
		if (dead)
			goto END;
		if (!food_available)
			GenerateFood(snake, snake_length, max_x, max_y, &food, &food_available);
		else
			mvprintw(food.y, food.x, "=");
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

void MoveSnake(Point *snake, Direction direction, int *snake_length, Point food, bool *food_available, int max_x, int max_y, bool *dead)
{
	int old_head_x = snake[0].x, old_head_y = snake[0].y;
	int new_head_x, new_head_y;
	new_head_x = old_head_x + direction.x_direction;
	new_head_y = old_head_y + direction.y_direction;

	// detect collision
	if (new_head_x < 0 || new_head_x > max_x || new_head_y < 0 || new_head_y > max_y)
	{
		*dead = true;
		return;
	}

	if (new_head_x == food.x && new_head_y == food.y)
	{
		*food_available = false;
		(*snake_length)++;
		snake = realloc(snake, (*snake_length) * sizeof(Point)); // extend the snake by 1
	}

	for (int i = *snake_length - 1; i > 0; --i)
	{
		snake[i].x = snake[i-1].x;
		snake[i].y = snake[i-1].y;
	}
	snake[0].x = new_head_x;
	snake[0].y = new_head_y;

	for (int i = 0; i < *snake_length; ++i)
		mvprintw(snake[i].y, snake[i].x, "o");

}

void GenerateFood(Point *snake, int snake_length, int max_x, int max_y, Point *food, bool *food_available)
{
	bool food_on_snake = true;

	while (food_on_snake)
	{
		food->x = rand() % max_x;
		food->y = rand() % max_y;
		for (int i = 0; i < snake_length; ++i)
			if (food->x == snake[i].x && food->y == snake[i].y)
				food_on_snake = true;
		food_on_snake = false;
	}

	*food_available = true;
	mvprintw(food->y, food->x, "=");
}
