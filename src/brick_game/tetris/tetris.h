#include <stdlib.h>
#include <time.h>

#include "../brick_game.h"

typedef struct {
  int** field;
  int** next;
  int score;
  int high_score;
  int level;
  int speed;
  // 2. int** current_figure;
  // 3. int x;
  // 4. int y;
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

int* tetris_next_row0(void);
