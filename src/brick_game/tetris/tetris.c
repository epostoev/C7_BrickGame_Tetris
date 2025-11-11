#include "tetris.h"

TetrisState_t* getTetrisInfo() {
  // Создание массива field;
  static int field[20][10] = {0};
  static int* rows[20];
  for (int i = 0; i < 20; i++) {
    rows[i] = field[i];
  }

  // Создание массива next;
  static int next[4][4] = {0};
  static int* next_rows[4];
  for (int i = 0; i < 4; i++) {
    next_rows[i] = next[i];
  }

  // Создание массива current
  static int current[4][4] = {0};
  static int* current_rows[4];
  for (int i = 0; i < 4; i++) {
    current_rows[i] = current[i];
  }

  // static TetrisState_t tetris_info = {
  //     .field = rows,
  //     .next = next_rows,
  //     .current = current_rows,
  //     .score = 1,
  //     .high_score = 2,
  //     .level = 3,
  //     .speed = 4,
  //     .fsm = kStart,
  //     .update_interval = 1000,
  // };

  // return &tetris_info;
  static TetrisState_t* ptr_tetris_info = NULL;
  if (ptr_tetris_info == NULL) {
    static TetrisState_t tetris_info = {
        .field = rows,
        .next = next_rows,
        .current = current_rows,
        .score = 1,
        .high_score = 2,
        .level = 3,
        .speed = 4,
        .fsm = kStart,
        .update_interval = 1000,
    };
    tetris_info.last_tick = currentTimeMs();
    ptr_tetris_info = &tetris_info;
  }

  return ptr_tetris_info;
}

void clearNext() {
  TetrisState_t* state = getTetrisInfo();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state->next[i][j] = 0;
    }
  }
}

void drawFigureI(int** next) {
  next[2][0] = 1;  // . . . .
  next[2][1] = 1;  // . . . .
  next[2][2] = 1;  // [][][][]
  next[2][3] = 1;  // . . . .
};
void drawFigureJ(int** next) {
  next[1][0] = 1;  // . . . .
  next[2][0] = 1;  // []. . .
  next[2][1] = 1;  // [][][].
  next[2][2] = 1;  // . . . .
};
void drawFigureL(int** next) {
  next[1][2] = 1;  // . . . .
  next[2][0] = 1;  // . . [].
  next[2][1] = 1;  // [][][].
  next[2][2] = 1;  // . . . .
};
void drawFigureO(int** next) {
  next[1][1] = 1;  // . . . .
  next[1][2] = 1;  // . [][].
  next[2][1] = 1;  // . [][].
  next[2][2] = 1;  // . . . .
};
void drawFigureS(int** next) {
  next[1][1] = 1;  // . . . .
  next[1][2] = 1;  // . [][].
  next[2][0] = 1;  // [][]. .
  next[2][1] = 1;  // . . . .
};
void drawFigureT(int** next) {
  next[1][1] = 1;  // . . . .
  next[2][0] = 1;  // . []. .
  next[2][1] = 1;  // [][][] .
  next[2][2] = 1;  // . . . .
};
void drawFigureZ(int** next) {
  next[1][0] = 1;  // . . . .
  next[1][1] = 1;  // [][] . .
  next[2][1] = 1;  // . [][].
  next[2][2] = 1;  // . . . .
};

void generateFigure() {
  TetrisState_t* state = getTetrisInfo();
  TypeFigure_t type_figure;
  void (*const get_figure[count_figure])(int**) = {
      drawFigureO, drawFigureI, drawFigureL, drawFigureJ,
      drawFigureS, drawFigureT, drawFigureZ};
  static bool next_empty = true;
  if (next_empty) {
    type_figure = rand() % 7;
    get_figure[type_figure](state->next);
    next_empty = false;
  }
  // Задаем начальные координаты x, y. (для current)
  state->x = 4;
  state->y = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      // Копирование в current
      state->current[i][j] = state->next[i][j];
    }
  }
  clearNext();
  type_figure = rand() % 7;
  get_figure[type_figure](state->next);
}

bool isPointOutField(int x, int y) {
  return (x < 0 || x >= 10 || y < 0 || y >= 20);
}

void addCurrentInField() {
  TetrisState_t* state = getTetrisInfo();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (state->current[i][j]) {
        int x = state->x + j;
        int y = state->y + i;
        if (isPointOutField(x, y) == false) {
          state->field[y][x] = state->current[i][j];
        }
      }
    }
  }
}

void checkFullLines() {
  TetrisState_t* state = getTetrisInfo();
  for (int i = 0; i < 20; i++) {
    int cnt = 0;
    for (int j = 0; j < 10; j++) {
      if (state->field[i][j]) {
        cnt++;
      }
    }
    // Есть ли заполненная линия, то
    mvprintw(31, 1, "cnt_666 = %d  ", cnt);
    if (cnt == 10) {
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < 10; j++) {
          // Убрать линию, сдвинуть все что выше
          state->field[k][j] = state->field[k - 1][j];
        }
      }
      memset(state->field[0], 0, 10);
    }
  }
}

GameInfo_t updateCurrentState() {
  // Создание массива state_info;
  GameInfo_t current_state = {0};
  TetrisState_t* state = getTetrisInfo();

  mvprintw(22, 1, "fsm = %d (0=Start, 1=Pause, 3=Move)", state->fsm);
  mvprintw(23, 1, "timeToShift = %d", timeToShift);

  if (state->fsm == kMove) {
    // bool should_shift = timeToShift;
  }

  if (state->fsm == kMove && timeToShift()) {
    // clearCurrent();  //  Убираем фигуру с поля
    if (false == moveFigureDown()) {
      // Есть ли заполненные линии
      checkFullLines();
      // for (int i = 0; i < 20; i++) {
      //   int cnt = 0;
      //   for (int j = 0; j < 10; j++) {
      //     if (state->field[i][j]) {
      //       cnt++;
      //     }
      //   }
      //   // Есть ли заполненная линия, то
      //   mvprintw(31, 1, "cnt_1 = %d  ", cnt);
      //   if (cnt == 10) {
      //     for (int k = i; k > 0; k--) {
      //       for (int j = 0; j < 10; j++) {
      //         // Убрать линию, сдвинуть все что выше
      //         state->field[k][j] = state->field[k - 1][j];
      //       }
      //     }
      //     memset(state->field[0], 0, 10);
      //   }
      // }

      // Начислить очки
      // Обновить уровень и обновить скорость
      // Проверка на завершение игры
      generateFigure();
      clearCurrent();
    }
  }
  // for (int i = 0; i < 20; i++) {
  //   for (int j = 0; j < 10; j++) {
  //     mvprintw(i + 40, j * 2, "%d", state->field[i][j]);
  //   }
  // }

  current_state.score = state->score;
  current_state.high_score = state->high_score;
  current_state.level = state->level;
  current_state.speed = state->speed;
  current_state.field = state->field;
  current_state.next = state->next;

  return current_state;
}

void clearCurrent() {
  TetrisState_t* state = getTetrisInfo();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (state->current[i][j]) {
        state->field[i + state->y][j + state->x] = 0;
      }
    }
  }
}

bool canPlaceAt(const TetrisState_t* state, int nx, int ny) {
  bool ok = true;
  for (int i = 0; i < 4 && ok; ++i) {
    for (int j = 0; j < 4 && ok; ++j) {
      if (state->current[i][j]) {
        int x = nx + j, y = ny + i;
        if (isPointOutField(x, y) || state->field[y][x]) {
          ok = false;  // 1. Переименовать переменную ok
        }
      }
    }
  }
  return ok;
}

void moveFigureLeft() {
  TetrisState_t* state = getTetrisInfo();
  clearCurrent();         // убрать старое положение
  int nx = state->x - 1;  // кандидатная позиция
  if (canPlaceAt(state, nx, state->y)) {
    state->x = nx;  // ок — фиксируем
  }
  addCurrentInField();  // дорисовать обратно
}

void moveFigureRight() {
  TetrisState_t* state = getTetrisInfo();
  clearCurrent();
  int nx = state->x + 1;
  if (canPlaceAt(state, nx, state->y)) {
    state->x = nx;
  }
  addCurrentInField();
}

bool moveFigureDown() {
  TetrisState_t* state = getTetrisInfo();
  clearCurrent();
  int ny = state->y + 1;
  bool moved = false;  // !!!! Обратить внимание !!!!
  if (canPlaceAt(state, state->x, ny)) {
    state->y = ny;
    moved = true;
  } else {
    // Фигура достигла конца - здесь будет логика "залочить и создать новую"
    // lockCurrentAndSpawnNext();
  }
  addCurrentInField();
  mvprintw(24, 1, "isCanMoveDown = %d  ", moved);
  return moved;
}

// Поворот фигуры на 90 градусов по часовой стрелке
void rotateFigure() {
  TetrisState_t* state = getTetrisInfo();

  // Создаём временный массив для повёрнутой фигуры
  int rotated[4][4] = {0};

  // Алгоритм поворота: rotated[i][j] = current[3-j][i]
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      rotated[i][j] = state->current[3 - j][i];
    }
  }

  // Сохраняем старую фигуру на случай отката
  // 1. Написать функцию копирования двумерного массива 4*4
  int old_current[4][4];
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      old_current[i][j] = state->current[i][j];  // Функция комирования
    }
  }

  // Убираем текущую фигуру с поля
  clearCurrent();

  // Применяем поворот
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state->current[i][j] = rotated[i][j];  // Функция комирования
    }
  }

  // Проверяем, можем ли разместить повёрнутую фигуру
  if (!canPlaceAt(state, state->x, state->y)) {
    // Если нельзя, откатываем поворот
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        state->current[i][j] = old_current[i][j];  // Функция комирования
      }
    }
  }

  // Рисуем фигуру обратно на поле
  addCurrentInField();
}

bool timeToShift() {
  TetrisState_t* game = getTetrisInfo();
  unsigned long now = currentTimeMs();

  mvprintw(28, 1, "now = %lu", now);
  mvprintw(29, 1, "last_tick = %lu", game->last_tick);
  mvprintw(30, 1, "dif = %lu", now - game->last_tick);

  if (now - game->last_tick >= game->update_interval) {
    game->last_tick = now;
    return true;  // FIXME
  }
  return false;
}

unsigned long currentTimeMs() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (unsigned long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

void userInput(UserAction_t action, bool hold) {
  if (hold == false) {
    return;
  }
  // Добавить конечный автомат;
  TetrisState_t* state = getTetrisInfo();

  mvprintw(25, 1, "action = %d, hold = %d", action, hold);
  // switch (action) {
  //   case Start:
  //     generateFigure();
  //     break;
  //   case Up:
  //     state->level += 1;
  //     break;
  //   case Down:
  //     if (false == moveFigureDown()) {
  //       generateFigure();
  //     }
  //     break;
  //   case Left:
  //     moveFigureLeft();
  //     break;
  //   case Right:
  //     moveFigureRight();
  //     break;
  //   case Action:
  //     rotateFigure();
  //     // Перенос на поле field и генерация следующей фигру в поле next
  //     break;
  //   default:
  //     break;
  // }
  switch (state->fsm) {
    case kStart:
      if (action == Start) {
        mvprintw(26, 1, "START pressed!Genereiting..... ");
        generateFigure();
        addCurrentInField();
        state->fsm = kMove;
        mvprintw(27, 1, "fsm set to kMove (%d)", state->fsm);
      }
      break;
    case kMove:
      if (action == Down) {
        // clearCurrent();
        if (moveFigureDown() == false) {
          // addCurrentInField();
          // generateFigure();
        };
        // addCurrentInField();
      } else if (action == Left) {
        moveFigureLeft();
      } else if (action == Right) {
        moveFigureRight();
      } else if (action == Action) {
        rotateFigure();
      }
      break;
    default:
      break;
  }
}

// Создать next на front[4][4] and back
// Создать фигуры
