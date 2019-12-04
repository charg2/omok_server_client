#include "stdafx.h"

#define BOARD_SIZE 19
#define WHITE 1
#define BLACK 2

//int now_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
int now_board[BOARD_SIZE][BOARD_SIZE] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
int gameStatus = 0;
int gameTurn = BLACK;

void resetBoard();
int setStone(int[], int);
int checkFiveInRange(int[]);
int checkFive(int[]);

void resetBoard() {
	for (int i = 0; i < BOARD_SIZE; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			now_board[i][j] = 0;
		}
	}
	gameTurn = BLACK;
}

int setStone(int position[], int color) {
	//Return 1 : Success 0 : Fail
	int resultCode;
	if (now_board[position[1]][position[0]] != 0) {
		resultCode = 0;
	} else {
		now_board[position[1]][position[0]] = color;
		resultCode = 1;
		//Check Win
		int result = checkFiveInRange(position);
		if (result) {
			gameStatus = result;
		} else {
			if (gameTurn == BLACK) {
				gameTurn = WHITE;
			} else {
				gameTurn = BLACK;
			}

		}
	}
	return resultCode;
}


int checkFiveInRange(int position[]) {
	for (int i = -2; i <= 2; i++) {
		int positionTemp[2] = { position[0] - i, position[1] };
		int result = checkFive(positionTemp);
		if (result) {
			return result;
		}
	}

	for (int i = -2; i <= 2; i++) {
		int positionTemp[2] = { position[0], position[1] - i };
		int result = checkFive(positionTemp);
		if (result) {
			return result;
		}
	}

	for (int i = -2; i <= 2; i++) {
		int positionTemp[2] = { position[0] - i, position[1] - i };
		int result = checkFive(positionTemp);
		if (result) {
			return result;
		}
	}
	for (int i = -2; i <= 2; i++) {
		int positionTemp[2] = { position[0] - i, position[1] + i };
		int result = checkFive(positionTemp);
		if (result) {
			return result;
		}
	}
	return 0;
}

int checkFive(int position[]) {
	//Return ColorCode : Win 0 : Nothing
	/*
	//prevent Out of Range
	if ((position[0] < 2 || position[0] > BOARD_SIZE - 2) || (position[1] < 2 || position[1] > BOARD_SIZE - 2)) {
		return 0;
	}
	*/
	//Check Nothing
	if (now_board[position[1]][position[0]] == 0) {
		return 0;
	}

	//Get Center Color
	int colorTemp = now_board[position[1]][position[0]];

	//Check Five (Vertical, Horizon, Diagonal)
	int cnt = 0;
	for (int i = -2; i <= 2; i++) {
		if (now_board[position[1] - i][position[0]] == colorTemp) {
			cnt++;
		}
	}
	if (cnt == 5) {
		return colorTemp;
	}

	cnt = 0;
	for (int i = -2; i <= 2; i++) {
		if (now_board[position[1]][position[0] - i] == colorTemp) {
			cnt++;
		}
	}
	if (cnt == 5) {
		return colorTemp;
	}

	cnt = 0;
	for (int i = -2; i <= 2; i++) {
		if (now_board[position[1] - i][position[0] - i] == colorTemp) {
			cnt++;
		}
	}
	if (cnt == 5) {
		return colorTemp;
	}

	cnt = 0;
	for (int i = -2; i <= 2; i++) {
		if (now_board[position[1] - i][position[0] + i] == colorTemp) {
			cnt++;
		}
	}
	if (cnt == 5) {
		return colorTemp;
	}

	return 0;
}