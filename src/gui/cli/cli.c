#include "cli.h"

void initNcurses(void) {
  initscr();  // инициализация ncurses
  cbreak();   // отключение буферизации ввода
  keypad(stdscr, true);  // включение обработки функциональных клавищ)
  noecho();  // отключение отображения вводимых символов
  curs_set(0);  // отключение отображения курсора
  timeout(100);
}

void gameLoop(void) {
  UserAction_t key_action;
  GameInfo_t state_info;
  bool run_game = true;
  bool hold;
  //   int cnt = 0;
  while (run_game) {
    hold = getAction(&key_action);      // <--
    userInput(key_action, hold);        // -->
    state_info = updateCurrentState();  // <--
    if (state_info.field == NULL) {
      run_game = false;
    } else {
      drawStateInfo(state_info);  // -->
    }
  }
}

// 5. Функция отрисовки статистики с данными
void drawInfo(GameInfo_t state_info) {
  // Отрисовка статистики с данными
  mvprintw(0, 21, "Score = %d", state_info.score);
  mvprintw(1, 21, "High score = %d", state_info.high_score);
  mvprintw(2, 21, "Level = %d", state_info.level);
  mvprintw(3, 21, "Speed = %d", state_info.speed);
}
// 6. Функция отрисовки легенды
void drawLegend(void) {
  char *legenda[11] = {
      "________________",  "| Process | Key ",  "|---------|-----",
      "|  START  | Enter", "|  EXIT   | Q",     "|  PAUSE  | P",
      "|   UP    | ^",     "|  DOWN   | V",     "|  RIGHT  | ->",
      "|  LEFT   | <-",    "|  ACTION | Space",
  };
  for (int i = 0; i < 11; i++) {
    mvprintw(i + 6, 30, legenda[i]);
  }
}

void drawStateInfo(GameInfo_t state_info) {
  // отрисовака поля
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      if (state_info.field[i][j]) {
        mvprintw(i, j * 2, "%s", "[]");
      } else {
        mvprintw(i, j * 2, "%s", ". ");
      }
    }
  }
  // Отрисовка next;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (state_info.next[i][j]) {
        mvprintw(i + 7, (21 + j * 2), "%s", "[] ");
      } else {
        mvprintw(i + 7, (21 + j * 2), "%s", ". ");
      }
    }
  }
  // 5. Отрисовка статистики с данными
  drawInfo(state_info);
  // 6. Функция отрисовки легенды
  drawLegend();
}

bool getAction(UserAction_t *key_action) {
  int signal = getch();
  bool return_err = false;
  if (signal != ERR) {
    return_err = true;
    switch (signal) {
      case 10:
      case 13:
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
        return_err = false;
        break;
    }
  }
  return return_err;
}
