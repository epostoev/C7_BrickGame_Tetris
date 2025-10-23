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
  static TetrisState_t tetris_info = {.field = rows,
                                      .next = next_rows,
                                      .current = current_rows,
                                      .score = 1,
                                      .high_score = 2,
                                      .level = 3,
                                      .speed = 4};

  return &tetris_info;
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
    refresh();
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
  refresh();
}

void addCurrentInField() {
  TetrisState_t* state = getTetrisInfo();
  // Переносим фигуру из current на field с учетом координат
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      // Копирование в current
      state->field[i + state->y][j + state->x] = state->current[i][j];
    }
  }
}

GameInfo_t updateCurrentState() {
  // Создание массива state_info;
  GameInfo_t current_state = {0};

  TetrisState_t* state = getTetrisInfo();

  addCurrentInField();

  current_state.score = state->score;
  current_state.high_score = state->high_score;
  current_state.level = state->level;
  current_state.speed = state->speed;
  current_state.field = state->field;
  current_state.next = state->next;
  mvprintw(23, 1, "x= %d", state->x);
  mvprintw(24, 1, "y= %d", state->y);
  return current_state;
}

void clearCurrent() {
  TetrisState_t* state = getTetrisInfo();
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      state->field[i + state->y][j + state->x] = 0;
    }
  }
}


static bool canPlaceAt(const TetrisState_t *st, int nx, int ny) {
  bool ok = true;
  for (int i = 0; i < 4 && ok; ++i) {
    for (int j = 0; j < 4 && ok; ++j) {
      if (st->current[i][j]) {
        int x = nx + j, y = ny + i;
        if (x < 0 || x >= 10 || y < 0 || y >= 20) 
        {ok = false;
        }
        else if (st->field[y][x])               ok = false;
      }
    }
  }
  return ok;
}


void moveFigureLeft() {
  TetrisState_t* st = getTetrisInfo();
  clearCurrent();                         // убрать старое положение
  int nx = st->x - 1;                     // кандидатная позиция
  if (canPlaceAt(st, nx, st->y)) {
    st->x = nx;                           // ок — фиксируем
  }
  addCurrentInField();                    // дорисовать обратно
}


// void moveFigureLeft(){
//   TetrisState_t* state = getTetrisInfo();
//   // Стереть текущее положение фигуры на field
//   clearCurrent();
//   // изменение координат по x на -1 пиксель
//   state->x = state->x - 1;
//   // 2.Проверка можем ли мы находиться в этих координатах
//   // если да, то addCurrentInField()
//   // иначе откатываем изменение координат обратно   state->x = state->x + 1;
//   // и отрисовываем обратно
// }


void moveFigureRight() {
  TetrisState_t* st = getTetrisInfo();
  clearCurrent();
  int nx = st->x + 1;
  if (canPlaceAt(st, nx, st->y)) {
    st->x = nx;
  }
  addCurrentInField();
}

// void moveFigureRight(){
//   TetrisState_t* state = getTetrisInfo();
//   clearCurrent();
//   // изменение координат по x на +1 пиксель
//   state->x = state->x + 1;
//   // 3.Проверка можем ли мы находиться в этих координатах
//   // если да, то addCurrentInField()
//   // иначе откатываем изменение координат обратно   state->x = state->x - 1;
//   // и отрисовываем обратно
// }


void moveFigureDown() {
  TetrisState_t* st = getTetrisInfo();
  clearCurrent();
  int ny = st->y + 1;
  if (canPlaceAt(st, st->x, ny)) {
    st->y = ny;                           // можем опуститься
  } else {
    // сюда позже ставится "залочить" фигуру и сгенерировать следующую
    // lockCurrentAndSpawnNext();
  }
  addCurrentInField();
}

//  void moveFigureDown(){
//   TetrisState_t* state = getTetrisInfo();
//   clearCurrent();
//   // изменение координат по y на +1 пиксель
//   state->y = state->y + 1;
//   // 3.Проверка можем ли мы находиться в этих координатах
//   // если да, то addCurrentInField()
//   // иначе откатываем изменение координат обратно   state->y = state->y - 1;
//   // и отрисовываем обратно
// }

// 4. Подумать как узнать конечное положение фигуры для генерации следующей фигуры


void userInput(UserAction_t action, bool hold) {
  if (hold == false) {
    return;
  }
  // Добавить конечный автомат;
  TetrisState_t* state = getTetrisInfo();
  // 8.по нажатию клавиши отрисовывать фигуру в next
  switch (action) {
    case Up:
      state->level += 1;
      break;
  // 1. Добаваить клавишу вниз (на 1 пиксель) (Готово)
    case Down:
      moveFigureDown();
      break;
    case Left:
      moveFigureLeft();
      break;
    case Right:
      moveFigureRight();
      break;
    case Action:
      generateFigure();
      // Перенос на поле field и генерация следующей фигру в поле next
      break;
    default:
      break;
  }
}

// Создать next на front[4][4] and back
// Создать фигуры
