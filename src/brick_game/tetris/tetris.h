#include <ncurses.h>  // УБРАТЬ ПРИ СДАЧЕ
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "../brick_game.h"

typedef struct {
  int cnt1;
  int cnt2;
} My_Counter;

typedef enum {
  kStart,
  kPause,
  kMove,
  kGameOver,
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
  int pause;

  // статистика игры
  int lines_cleared;
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

GameInfo_t getGameInfo();


unsigned long currentTimeMs();
bool timeToShift();
bool moveFigureDown();
void clearCurrent();
void addCurrentInField();
void generateFigure();
bool isPointOutField(int x, int y);
bool canPlaceAt(const TetrisState_t* state, int nx, int ny);
void rotateFigure();
void moveFigureLeft();
void moveFigureRight();
TetrisState_t* getTetrisInfo();
// Добавать обьявление функций