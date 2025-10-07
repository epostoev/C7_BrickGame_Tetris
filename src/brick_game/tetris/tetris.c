#include "tetris.h"

typedef struct {
  int level;
} TetrisState;

TetrisState* getTetrisInfo () {
	static TetrisState tetris_info = { .level = 1 };
	return &tetris_info;
}

GameInfo_t updateCurrentState() {
	// 4.Создание массива field;
	static int field[20][10] = {0};
	field [6][8] = 1;
	static int* rows [20];
	for (int i = 0; i < 20; i++){
		rows[i] = field[i];
	}

	  // 5.Создание массива next;

	GameInfo_t current_state;
	TetrisState* state = getTetrisInfo();
	current_state.level = state->level;
	current_state.field = rows;
	// 6.продлить на все остальные поля
	// 7.current_state.next = next_rows;

	return current_state;
}

void userInput(UserAction_t action, bool hold) {
	if (hold == false) {
		return;
	}
	TetrisState* state = getTetrisInfo();
	// 8.по нажатию клавиши отрисовывать фигуру в next
	switch (action) {
	  case Up: 
		state->level += 1;
		break;
	  default:
		break;		
	}
}


// Создать next на front[4][4] and back 
// Создать фигуры
