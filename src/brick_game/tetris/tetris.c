#include "tetris.h"

GameInfo_t updateCurrentState() {
	int field[20][10];
	int *rows [20];
	for (int i = 0; i < 20; i++){
		rows[i] = field[i];
	}
	GameInfo_t current_state;
	current_state.level = 666;
	current_state.field = rows;
	return current_state;
}
void userInput(UserAction_t action, bool hold) {

}
