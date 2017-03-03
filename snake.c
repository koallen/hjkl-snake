#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 140000

typedef struct {
	int x_direction;
	int y_direction;
} Direction;

typedef struct {
	int x;
	int y;
} Point;

void PrintSnake(Point *snake, Direction direction, int snake_length);

int main()
{
	/*
	 * initialize ncurses screen
	 */
	initscr();
	noecho();
	curs_set(FALSE);
	nodelay(stdscr, TRUE); // don't block on getch() call
	int max_x = 0, max_y = 0;
	Point *snake = (Point *)malloc(sizeof(Point));
	int snake_length = 1;
	snake[0].x = 0;
	snake[0].y = 0;
	Direction direction = {0, 0};

	/*
	 * main loop
	 */
	getmaxyx(stdscr, max_y, max_x); // get window size
	mvprintw(max_y, 0, "Quit the game with 'q'");
	max_y--;
	mvprintw(snake[0].x, snake[0].y, "o");
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
		PrintSnake(snake, direction, snake_length);
		usleep(DELAY);
	}

	/*
	 * finalize ncurses screen
	 */
END:
	endwin();

	return EXIT_SUCCESS;
}

void PrintSnake(Point *snake, Direction direction, int snake_length)
{
	int old_head_x = snake[0].x, old_head_y = snake[0].y;

	for (int i = snake_length - 1; i > 0; --i)
	{
		snake[i].x = snake[i-1].x;
		snake[i].y = snake[i-1].y;
	}
	snake[0].x = old_head_x + direction.x_direction;
	snake[0].y = old_head_y + direction.y_direction;

	for (int i = 0; i < snake_length; ++i)
		mvprintw(snake[i].y, snake[i].x, "o");
	refresh();
}
