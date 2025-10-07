#include "cli.h"

void initNcurses() {
  initscr();             // инициализация ncurses
  cbreak();              // отключение буферизации ввода
  keypad(stdscr, true);  // включение обработки функциональных клавищ)
  noecho();              // отключение отображения вводимых символов
  curs_set(0);           // отключение отображения курсора
  timeout(100);
}

void gameLoop() {
  UserAction_t key_action;
  GameInfo_t state_info;
  bool run_game = true;
  bool hold;
  while (run_game) {
    hold = getAction(&key_action);
    userInput(key_action, hold);
    state_info = updateCurrentState();
    drawStateInfo(state_info);
  }
}

void drawStateInfo(GameInfo_t state_info) {
  // отрисовака поля
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      if (state_info.field[i][j]) {
        mvprintw(i, j , "%s", "[]");
      } else {
        mvprintw(i, j , "%s", ". ");
      }
    }
  }
  // 1.Отрисовка next;

  // 2.Отрисовка статистики с данными
  mvprintw(0, 20, "level = %d", state_info.level);

  // 3.Отрисовка легенды
}

bool getAction(UserAction_t *key_action) {
  int signal = getch();
  mvprintw(21, 1, "code=%d hex=%#x", signal, signal);
  bool return_err = false;
  if (signal != ERR) {
    return_err = true;
    switch (signal) {
      case KEY_ENTER:
        *key_action = Start;
        break;
      case KEY_P_LOWER:
        *key_action = Pause;
        break;
      case KEY_Q_LOWER:
        *key_action = Terminate;
        break;
      case KEY_LEFT:
        *key_action = Left;
        break;
      case KEY_RIGHT:
        *key_action = Right;
        break;
      case KEY_UP:
        *key_action = Up;
        break;
      case KEY_DOWN:
        *key_action = Down;
        break;
      case KEY_SPACE:
        *key_action = Action;
        break;
      default:
        break;
    }
  }
  return return_err;
}