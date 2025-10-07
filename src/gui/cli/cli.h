#include <ncurses.h>
#include "../../brick_game/brick_game.h"

#define KEY_P_LOWER 112
#define KEY_Q_LOWER 113
#define KEY_SPACE 32


void initNcurses();
void gameLoop();
bool getAction(UserAction_t *key_action);
void drawStateInfo (GameInfo_t state_info);