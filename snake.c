#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define DELAY 150000

typedef struct {
	int x_direction;
	int y_direction;
} Direction;

int main()
{
	/*
	 * initialize ncurses screen
	 */
	initscr();
	noecho();
	curs_set(FALSE);
	nodelay(stdscr, TRUE); // don't block on getch() call
	int max_x, max_y, x = 0, y = 0;
	Direction direction = {0, 0};

	/*
	 * main loop
	 */
	mvprintw(y, x, "o");
	refresh();
	while (1)
	{
		getmaxyx(stdscr, max_y, max_x); // update window border
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
		x = x + direction.x_direction;
		y = y + direction.y_direction;
		clear();
		mvprintw(y, x, "o");
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
