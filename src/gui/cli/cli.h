#include <ncurses.h>
#include "../../brick_game/brick_game.h"

void initNcurses();
void gameLoop();
bool getAction(UserAction_t *key_action);
void drawStateInfo (GameInfo_t state_info);