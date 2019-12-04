//#include<stdio.h>
//#include<Windows.h>
//#include<conio.h>
//
//#define BOARD_SIZE 19
//#define WHITE 1
//#define BLACK 2
//
//int now_position[2] = { 8, 8 };
//extern int now_board[][BOARD_SIZE];
//extern int gameStatus;
//extern int gameTurn;
//extern int AI_init(int, int[]);
//void key_pressed();
//void DrawScreen(int x[][BOARD_SIZE]);
//void moveCursor(int, int);
//void move_position(int);
//extern int setStone(int[], int);
//
//int main() {
//	system("mode con: cols=100 lines=41");
//	DrawScreen(now_board);
//	while (1) {
//		if (kbhit()) {key_pressed();} // Key Input Check
//		moveCursor(0, 0);
//
//	}
//}
//
//void key_pressed() {
//	int key = getch();
//	switch (key) {
//		case 13:
//			//printf("ENTER");
//			if (gameStatus == 0) {
//				if (setStone(now_position, gameTurn)) {
//					if (gameStatus == 0) {
//						int ai_position[2] = { 0, 0 };
//						AI_init(gameTurn, ai_position);
//						while (setStone(ai_position, gameTurn));
//
//					}
//				}
//			}
//			break;
//		case 72:
//			//printf("UP");
//			move_position(1);
//			break;
//		case 75:
//			//printf("LEFT");
//			move_position(2);
//			break;
//		case 77:
//			//printf("RIGHT");
//			move_position(3);
//			break;
//		case 80:
//			//printf("DOWN");
//			move_position(4);
//			break;
//	}
//	DrawScreen(now_board);
//}
//
//void move_position(int key) {
//	switch (key) {
//		case 1: 
//			//UP
//			if (now_position[1] != 0) {
//				now_position[1]--;
//			}
//			break;
//		case 2:
//			if (now_position[0] != 0) {
//				now_position[0]--;
//			}
//			//LEFT
//			break;
//		case 3:
//			if (now_position[0] != (BOARD_SIZE - 1)) {
//				now_position[0]++;
//			}
//			//RIGHT
//			break;
//		case 4:
//			//DOWN
//			if (now_position[1] != (BOARD_SIZE - 1)) {
//				now_position[1]++;
//			}
//			break;
//	}
//}
//
//void moveCursor(int x, int y) {
//	COORD pos = {2 * x, y};
//	CONSOLE_CURSOR_INFO info;
//	info.dwSize = 100;
//	info.bVisible = FALSE; //Hide Cursor
//	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
//	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
//}
//
//void DrawScreen(int board[][BOARD_SIZE]) {
//	system("cls"); //ClearScreen
//	moveCursor(3, 1);
//	for (char i = 'A'; i < 'A' + BOARD_SIZE; i++) {
//		printf(" %2c ", i);
//	}
//	for (int i = 0; i < BOARD_SIZE; i++) {
//		printf("\n\n  %2d  ", i + 1);
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			if (now_position[0] == j && now_position[1] == i) {
//				printf(" × ");
//			} else if (board[i][j] == WHITE) {
//				printf(" ○ ");
//			} else if (board[i][j] == BLACK) {
//				printf(" ● ");
//			} else {
//				printf("    ");
//			}
//		}
//	}
//	moveCursor(42, 15);
//	printf("OMOK GAME");
//	moveCursor(42, 18);
//	printf("POSITION");
//	moveCursor(43, 20);
//	printf("%c%d", now_position[0] + 'A', now_position[1] + 1);
//	moveCursor(42, 24);
//	if (gameStatus) {
//		if (gameStatus == 1) {
//			printf("WHITE WIN!");
//		} else {
//			printf("BLACK WIN!");
//		}
//	}
//}