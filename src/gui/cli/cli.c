#include "cli.h"

void initNcurses() {
  initscr();  // инициализация ncurses
  cbreak();   // отключение буферизации ввода
  keypad(stdscr, true);  // включение обработки функциональных клавищ)
  noecho();  // отключение отображения вводимых символов
  curs_set(0);  // отключение отображения курсора
  timeout(100);
}

void gameLoop() {
  UserAction_t key_action;
  GameInfo_t state_info;
  bool run_game = true;
  while (run_game) {
    getAction(&key_action);
    userInput(key_action, false);
    state_info = updateCurrentState();
	drawStateInfo(state_info);
  }
}

void drawStateInfo (GameInfo_t state_info) {
	// отрисовака поля	
	for (int i = 0; i < 20; i++) {
		for(int j = 0; j < 10; j++){
			mvprintw(i, j, "%c", state_info.field[i][j]);
		}
	}

	mvprintw(0, 0, "level = %d", state_info.level);
}

bool getAction(UserAction_t *key_action) {
  int signal = getch();
  bool return_err = false;
  if (signal != ERR) {
    return_err = true;
    switch (signal) {
      case KEY_UP:
        *key_action = Up;
        break;
      case KEY_LEFT:
        *key_action = Left;
        break;
      case KEY_RIGHT:
        *key_action = Right;
        break;
      default:
        break;
    }
  }
  return return_err;
}