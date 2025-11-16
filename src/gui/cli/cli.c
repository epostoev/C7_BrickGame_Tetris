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
  bool hold;
  int cnt = 0;
  while (run_game) {
    hold = getAction(&key_action);	// <--
    mvprintw(25, 28, "| hold = %d", hold);
    mvprintw(26, 28, "| cnt = %d", cnt++);
    userInput(key_action, hold);	// -->	
    state_info = updateCurrentState();	// <--
    drawStateInfo(state_info);			// -->
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
  mvprintw(6, 21, "%s", "Next:");
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (state_info.next[i][j]) {
        mvprintw(i + 7, (21 + j * 2), "%s", "[] ");
      } else {
        mvprintw(i + 7, (21 + j * 2), "%s", ". ");
      }
    }
  }

  // Отрисовка статистики с данными
  mvprintw(0, 21, "Score = %d", state_info.score);
  mvprintw(1, 21, "High score = %d", state_info.high_score);
  mvprintw(2, 21, "Level = %d", state_info.level);
  mvprintw(3, 21, "Speed = %d", state_info.speed);

  // Отрисовка легенды
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
        mvprintw(25, 1, "      ");
        mvprintw(25, 1, "Start");
        break;
      case KEY_P_LOWER:
        *key_action = Pause;
        mvprintw(25, 1, "      ");
        mvprintw(25, 1, "Pause");
        break;
		case KEY_Q_LOWER:
        *key_action = Terminate;
        break;
		case KEY_LEFT:
        *key_action = Left;
        mvprintw(25, 1, "      ");
        mvprintw(25, 1, "Left");
        mvprintw(26, 1, "key_action = %p", (void*)key_action);
        mvprintw(27, 1, "key_action_number = %d", *key_action);
        break;
		case KEY_RIGHT:
        *key_action = Right;
        mvprintw(25, 1, "      ");
        mvprintw(25, 1, "Right");
        mvprintw(26, 1, "key_action = %p", (void*)key_action);
        mvprintw(27, 1, "key_action_number = %d", *key_action);
        break;
		case KEY_UP:
        *key_action = Up;
        break;
		case KEY_DOWN:
        *key_action = Down;
        mvprintw(25, 1, "      ");
        mvprintw(25, 1, "Down");
        break;
		case KEY_SPACE:
        *key_action = Action;
        mvprintw(25, 1, "      ");
        mvprintw(25, 1, "Action");
        break;
      default:
        return_err = false;
        break;
    }
  }
  return return_err;
}