#ifndef TETRIS_H
#define TETRIS_H

#include <stdio.h>
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

GameInfo_t getGameInfo(void);

unsigned long currentTimeMs(void);
bool timeToShift(void);
bool moveFigureDown(void);
void clearCurrent(void);
void addCurrentInField(void);
void generateFigure(void);
bool isPointOutField(int x, int y);
bool canPlaceAt(const TetrisState_t* state, int nx, int ny);
void rotateFigure(void);
void moveFigureLeft(void);
void moveFigureRight(void);
TetrisState_t* getTetrisInfo(void);
// Добавать обьявление функций

#endif  // TETRIS_H
