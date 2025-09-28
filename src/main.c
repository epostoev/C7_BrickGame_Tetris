#include "gui/cli/cli.h"

int main(void)
{
	// иницилизация/настройки функции ncurses
	initNcurses();
	// игровой цикл
	gameLoop();
	endwin();
	// обработка выхода
}