#include "tetris.h"

TetrisState_t* getTetrisInfo() {
  // 4.Создание массива field;
  static int field[20][10] = {0};
  static int* rows[20];
  for (int i = 0; i < 20; i++) {
    rows[i] = field[i];
  }

  // 5.Создание массива next;
  static int next[4][4] = {0};
  static int* next_rows[4];
  for (int i = 0; i < 4; i++) {
    next_rows[i] = next[i];
  }
  static TetrisState_t tetris_info = {.field = rows,
                                      .next = next_rows,
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
  next[2][0] = 1;				// . . . .
  next[2][1] = 1;				// . . . .
  next[2][2] = 1;				// [][][][]
  next[2][3] = 1;				// . . . .
};
void drawFigureJ(int** next) {
  next[1][0] = 1;				// . . . .
  next[2][0] = 1;				// []. . .
  next[2][1] = 1;				// [][][].
  next[2][2] = 1;				// . . . .
};
void drawFigureL(int** next) {
  next[1][2] = 1;				// . . . .
  next[2][0] = 1;				// . . [].
  next[2][1] = 1;				// [][][].
  next[2][2] = 1;       // . . . .
};
void drawFigureO(int** next) {
  next[1][1] = 1;				// . . . .
  next[1][2] = 1;				// . [][].
  next[2][1] = 1;				// . [][].
  next[2][2] = 1;				// . . . .
};
void drawFigureS(int** next) {
  next[1][1] = 1;				// . . . .
  next[1][2] = 1;				// . [][].
  next[2][0] = 1;				// [][]. .
  next[2][1] = 1;				// . . . .
};
void drawFigureT(int** next) {
  next[1][1] = 1;				// . . . .
  next[2][0] = 1;				// . []. .
  next[2][1] = 1;				// [][][] .
  next[2][2] = 1;				// . . . .
};
void drawFigureZ(int** next) {
  next[1][0] = 1;				// . . . .
  next[1][1] = 1;				// [][] . .
  next[2][1] = 1;				// . [][].
  next[2][2] = 1;				// . . . .
};

// 1. Дописать все фигуры;

void generateFigure() {
  TetrisState_t* state = getTetrisInfo();
  TypeFigure_t type_figure;
  void (*const get_figure[count_figure])(int**) = {drawFigureO, drawFigureI,
                                                   drawFigureL, drawFigureJ,
                                                   drawFigureS, drawFigureT,
                                                   drawFigureZ};
  static bool next_empty = true;
  if (next_empty) {
    type_figure = rand() % 7;
    get_figure[type_figure](state->next);
    refresh();

    next_empty = false;
  }
  // 5. задаем начальные координаты x, y. (для current)
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      // 6. копирование в current
      state->field[i][j] = state->next[i][j];
    }
  }
  clearNext();
  type_figure = rand() % 7;
  get_figure[type_figure](state->next);
  refresh();
}

GameInfo_t updateCurrentState() {
  // Создание массива state_info;
  GameInfo_t current_state = {0};
  TetrisState_t* state = getTetrisInfo();

  // 7.перенести фигуру из current на field с учетом координат
  // и проверкой можем ли мы находиться в этих координатах
  current_state.score = state->score;
  current_state.high_score = state->high_score;
  current_state.level = state->level;
  current_state.speed = state->speed;
  current_state.field = state->field;
  current_state.next = state->next;

  return current_state;
}

void userInput(UserAction_t action, bool hold) {
  if (hold == false) {
    return;
  }
  TetrisState_t* state = getTetrisInfo();
  // 8.по нажатию клавиши отрисовывать фигуру в next
  switch (action) {
    case Up:
      state->level += 1;
      break;
    case Down:
      state->level -= 1;
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
