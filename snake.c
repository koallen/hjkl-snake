#include <ncurses.h>
#include <unistd.h>

int main()
{
	initscr();
	noecho();
	curs_set(FALSE);

	mvprintw(0, 0, "Hello, world!");
	refresh();

	sleep(1);

	endwin();
}
