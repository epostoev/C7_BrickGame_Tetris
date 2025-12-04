#include "tetris.h"

TetrisState_t* getTetrisInfo() {
  // Создание массива field;
  static int field[20][10] = {0};
  static int* rows[20];
  // Создание массива next;
  static int next[4][4] = {0};
  static int* next_rows[4];
  // Создание массива current
  static int current[4][4] = {0}, *current_rows[4];
  static TetrisState_t* ptr_tetris_info = NULL;
  if (ptr_tetris_info == NULL) {
    for (int i = 0; i < 20; i++) {
      rows[i] = field[i];  // назначаем указатели на ряды массива field
    }
    for (int i = 0; i < 4; i++) {
      next_rows[i] = next[i];
    }
    for (int i = 0; i < 4; i++) {
      current_rows[i] = current[i];
    }
    static TetrisState_t tetris_info = {
        .field = rows,
        .next = next_rows,
        .current = current_rows,
        .score = 0,
        .high_score = 0,
        .level = 0,
        .speed = 0,
        .fsm = kStart,
        .update_interval = 1000,
    };
    FILE* fileHighScore = fopen("Record.txt", "r");
    if (fileHighScore != NULL) {
      fscanf(fileHighScore, "%d", &tetris_info.high_score);
      fclose(fileHighScore);
    } else {
      fileHighScore = fopen("Record.txt", "w");
      fprintf(fileHighScore, "%d", tetris_info.high_score);
      fclose(fileHighScore);
    }
    tetris_info.last_tick = currentTimeMs();
    ptr_tetris_info = &tetris_info;
    srand(time(NULL));
  }
  return ptr_tetris_info;
}

// 3. Функция очистки массива next
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
  state->y = -3;
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

bool isPointOutField(int x, int y) { return (x < 0 || x >= 10 || y >= 20); }
bool isPointInField(int x, int y) {
  return (x >= 0 && x < 10 && y >= 0 && y < 20);
}

void addCurrentInField() {
  TetrisState_t* state = getTetrisInfo();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (state->current[i][j]) {
        int x = state->x + j;
        int y = state->y + i;
        if (isPointInField(x, y) == true) {
          state->field[y][x] = state->current[i][j];
        }
      }
    }
  }
}

void sumScore() {
  TetrisState_t* state = getTetrisInfo();
  if (state->lines_cleared == 1) {
    state->score = state->score + 100;
  }
  if (state->lines_cleared == 2) {
    state->score = state->score + 300;
  }
  if (state->lines_cleared == 3) {
    state->score = state->score + 700;
  }
  if (state->lines_cleared == 4) {
    state->score = state->score + 1500;
  }
}

void checkFullLines() {
  TetrisState_t* state = getTetrisInfo();
  state->lines_cleared = 0;
  for (int i = 0; i < 20; i++) {
    int cnt = 0;
    for (int j = 0; j < 10; j++) {
      if (state->field[i][j]) {
        cnt++;
      }
    }
    // Есть ли заполненная линия, то
    if (cnt == 10) {
      state->lines_cleared++;
      for (int k = i; k > 0; k--) {
        for (int j = 0; j < 10; j++) {
          // Убрать линию, сдвинуть все что выше
          state->field[k][j] = state->field[k - 1][j];
        }
      }
      memset(state->field[0], 0, sizeof(int) * 10);
    }
  }
  // Увеличиваем score в зависимости сколько линий удалилось
  int old_score = state->score;

  sumScore();

  // Проверка на запись в поле high_score
  if (state->score > state->high_score) {
    state->high_score = state->score;

    FILE* fileHighScore = fopen("Record.txt", "w");
    if (fileHighScore != NULL) {
      // fscanf(fileHighScore, "%d", state->high_score);
      fprintf(fileHighScore, "%d", state->high_score);
      fclose(fileHighScore);
    }
  }
  int new_score = state->score;
  int old_lvl = old_score / 600;
  int new_lvl = new_score / 600;
  if (new_lvl > old_lvl && state->lines_cleared > 0 && state->level <= 10) {
    state->level++;
    state->speed++;
    state->update_interval = state->update_interval - 100;
  }
}

GameInfo_t updateCurrentState() {
  // Создание массива state_info;
  static My_Counter current = {0};
  TetrisState_t* state = getTetrisInfo();
  if (state->field != NULL) {  // Вызов Terminate
    if (state->fsm == kMove && timeToShift()) {
      if (false == moveFigureDown()) {
        // Есть ли заполненные линии
        // Начислить очки
        // Обновить уровень и обновить скорость
        checkFullLines();
        // Проверка на завершение игры
        // Если заполнен нулевой ряд,
        // Если в нулевом ряду есть хотя бы одна
        for (int i = 0; i < 10 && state->fsm != kGameOver; i++) {
          if (state->field[0][i] == 1) {
            state->fsm = kGameOver;
          }
        }
        if (state->fsm != kGameOver) {
          generateFigure();
          clearCurrent();
        }
      }
    }
  }
  return getGameInfo();
}

GameInfo_t getGameInfo() {
  GameInfo_t current_state = {0};
  TetrisState_t* state = getTetrisInfo();
  current_state.score = state->score;
  current_state.high_score = state->high_score;
  current_state.level = state->level;
  current_state.speed = state->speed;
  current_state.field = state->field;
  current_state.next = state->next;
  current_state.pause = state->pause;
  return current_state;
}
// 4. Функция очистки массива сгккуте
void clearCurrent() {
  TetrisState_t* state = getTetrisInfo();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (state->current[i][j]) {
        int x = state->x + j;
        int y = state->y + i;
        if (isPointInField(x, y) == true) {
          state->field[y][x] = 0;
        }
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
        if (isPointOutField(x, y) ||
            isPointInField(x, y) && state->field[y][x]) {
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
  }
  addCurrentInField();
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
  if (now - game->last_tick >= game->update_interval) {
    game->last_tick = now;
    return true;  // TODO: FIXME
  }
  return false;
}

unsigned long currentTimeMs() {
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return (unsigned long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

// 1. Функция очистки поля
void clearField() {
  TetrisState_t* state = getTetrisInfo();

  // Очитска поля 20х10
  for (int i = 0; i < 20; i++) {
    for (int j = 0; j < 10; j++) {
      state->field[i][j] = 0;
    }
  }
}

// 2. Функция сброса статистики
void restartGameStats() {
  TetrisState_t* state = getTetrisInfo();
  state->score = 0;
  state->level = 0;
  state->speed = 0;
  state->lines_cleared = 0;
  state->update_interval = 1000;
  state->x = 4;
  state->y = -4;
  state->last_tick = currentTimeMs();
}

void restartGame() {
  TetrisState_t* state = getTetrisInfo();

  // 1. Очищаем поле
  clearField();
  // 2. Сбрасываем статистику
  restartGameStats();
  // 3. Очищаем поле next
  clearNext();
  // 4. Очищаем массив current
  clearCurrent();

  generateFigure();
  addCurrentInField();
  state->fsm = kMove;
}

void userInput(UserAction_t action, bool hold) {
  if (hold == false) {
    return;
  }
  TetrisState_t* state = getTetrisInfo();
  switch (state->fsm) {
    case kStart:
    case kGameOver:
      if (action == Start) {
        restartGame();
      } else if (action == Terminate) {
        state->field = NULL;
      }
      break;
    case kMove:
      if (action == Down) {
        while (moveFigureDown()) {
        };
      } else if (action == Left) {
        moveFigureLeft();
      } else if (action == Right) {
        moveFigureRight();
      } else if (action == Action) {
        rotateFigure();
      } else if (action == Pause) {
        state->fsm = kPause;
        state->pause = 1;
      } else if (action == Terminate) {
        state->field = NULL;
      }
      break;
    case kPause:
      if (action == Pause) {
        state->fsm = kMove;
        state->pause = 0;
      } else if (action == Terminate) {
        state->field = NULL;
      }
      break;
    default:
      break;
  }
}