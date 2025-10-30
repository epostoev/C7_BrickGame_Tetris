#include <ncurses.h>  // УБРАТЬ ПРИ СДАЧЕ
#include <stdlib.h>
#include <time.h>

#include "../brick_game.h"

typedef enum {
  kStart,
  kPause,
  kMove,
} FiniteState_t;

typedef struct {
  int** field;
  int** next;
  int score;
  int high_score;
  int level;
  int speed;
  int** current;
  int x;
  int y;
  FiniteState_t fsm;
  unsigned long last_tick;
  unsigned long update_interval;
} TetrisState_t;

typedef enum {
  figure_I,
  figure_O,
  figure_L,
  figure_J,
  figure_S,
  figure_T,
  figure_Z,
  count_figure,
} TypeFigure_t;

unsigned long currentTimeMs();
bool timeToShift();
bool moveFigureDown();
// Добавать обьявление функций