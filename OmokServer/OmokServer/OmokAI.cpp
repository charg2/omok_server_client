//#include<stdlib.h>
//#include<time.h>
//
////Update : 17.06.14 00:37
//
//#define BOARD_SIZE 19
//#define WHITE 1
//#define BLACK 2
//
//int checkCapability(int[]);
//int AI_init(int, int[]);
//void continueousScore(int[][BOARD_SIZE], int[][BOARD_SIZE]);
//void extractBoard(int[][BOARD_SIZE], int, int[][BOARD_SIZE]);
//void sumArray(int[][BOARD_SIZE], int[][BOARD_SIZE], int[][BOARD_SIZE]);
//int getAbsPosition(int[][BOARD_SIZE], int[], int[][BOARD_SIZE], int[][BOARD_SIZE]);
//void filterDisable(int[][BOARD_SIZE]);
//int getBestPosition(int[][BOARD_SIZE], int[][BOARD_SIZE], int[][BOARD_SIZE], int[]);
//int getAbsPosition(int[][BOARD_SIZE], int[], int[][BOARD_SIZE], int[][BOARD_SIZE], int, int);
//int checkSameModel(int[][BOARD_SIZE], int[][BOARD_SIZE], int[], int[], int[], int);
//extern int checkFiveInRange(int[]);
//extern int checkFive(int[]);
//extern int now_board[][BOARD_SIZE];
//
//
////우선순위
////------------------------------------
////(중요)
////1. 내 돌이 4개인 지점을 찾는다.
////2. 상대방 돌이 4개인 지점을 찾는다.
////3. 내 돌이 3개인 지점을 찾는다.
////4. 상대방 돌이 3개인 지점을 찾는다.
////------------------------------------
////(기타)
////5. 내 돌의 연속성을 찾는다.
////->연속된 개수(4개 > 3개 > 2개 > 1개)에 따라서 점수부여
////->공격, 수비의 점수를 합쳐서 착수함
//
//
///*
////This methods are for debugging
//void printArray(int arr[][BOARD_SIZE]);
//int main() {
//	system("mode con: cols=100 lines=41");
//
//	int firstBoard[BOARD_SIZE][BOARD_SIZE];
//	continueousScore(now_board_temp, firstBoard);
//
//	//printArray(now_board_temp);
//	printArray(firstBoard);
//	return 0;
//}
//
//void printArray(int arr[][BOARD_SIZE]) {
//	for (int i = 0; i < BOARD_SIZE; i++) {
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			printf("%4d", arr[i][j]);
//		}
//		printf("\n\n");
//	}
//}
//
//void printLargerArray(int arr[][BOARD_SIZE + 2]) {
//	for (int i = 0; i < BOARD_SIZE + 2; i++) {
//		for (int j = 0; j < BOARD_SIZE + 2; j++) {
//			printf("%4d", arr[i][j]);
//		}
//		printf("\n\n");
//	}
//}
////End of Debugging Methods
//*/
//
//int AI_init(int color, int bestPosition[]) {
//	
//	int def_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
//	int atk_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
//
//	int def_score_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
//	int atk_score_board[BOARD_SIZE][BOARD_SIZE] = { 0 };
//
//	int com_color, play_color;
//	if (color == WHITE) {
//		com_color = WHITE;
//		play_color = BLACK;
//	} else {
//		com_color = BLACK;
//		play_color = WHITE;
//	}
//
//	extractBoard(now_board, com_color, atk_board);
//	extractBoard(now_board, play_color, def_board);
//
//	//STEP 1
//	if (getAbsPosition(now_board, bestPosition, atk_board, def_board, com_color, play_color)) {
//		return 1;
//	}
//	continueousScore(atk_board, atk_score_board);
//	continueousScore(def_board, def_score_board);
//
//	//STEP 2
//	int score_sum[BOARD_SIZE][BOARD_SIZE] = { 0 };
//	sumArray(atk_score_board, def_score_board, score_sum);
//	filterDisable(score_sum);
//	getBestPosition(score_sum, atk_score_board, def_score_board, bestPosition);
//
//	return 1;
//}
//
//int getRandomInt(int range) {
//	srand(time(NULL));
//	return (rand() % range + 1);
//}
//
//int getAbsPosition(int board[][BOARD_SIZE], int position[2], int atk_board[][BOARD_SIZE], int def_board[][BOARD_SIZE], int com_color, int play_color) {
//	//Check Five
//	for (int i = 0; i < BOARD_SIZE; i++) {
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			if (board[i][j] == 0) {
//				board[i][j] = WHITE;
//  				int positionTemp[2] = { j, i };
//				if (checkFiveInRange(positionTemp)) {
//					position[0] = positionTemp[0];
//					position[1] = positionTemp[1];
//					board[i][j] = 0;
//					return 1;
//				}
//
//				board[i][j] = BLACK;
//				if (checkFiveInRange(positionTemp)) {
//					position[0] = positionTemp[0];
//					position[1] = positionTemp[1];
//					board[i][j] = 0;
//					return 1;
//				}
//				board[i][j] = 0;
//			}
//		}
//	 }
//
//	//Check Four (2 + 1 or 1 + 2)
//	int modelA[6] = {0, 1, 1, 0, 1, 0};
//	int empty_position[2] = { 18, 18 };
//	for (int i = 0; i < BOARD_SIZE; i++) {
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			if (atk_board[i][j] == 0) {
//				int positionTemp[2] = { i, j };
//				int sameCode = checkSameModel(atk_board, def_board, positionTemp, modelA, empty_position, sizeof(modelA) / sizeof(int));
//				if (sameCode) {
//					if (board[empty_position[1]][empty_position[0]] == 0) {
//						position[0] = empty_position[0];
//						position[1] = empty_position[1];
//						return 1;
//					}
//				}
//			}
//		}
//	}
//	for (int i = 0; i < BOARD_SIZE; i++) {
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			if (def_board[i][j] == 0) {
//				int positionTemp[2] = { i, j };
//				int sameCode = checkSameModel(def_board, atk_board, positionTemp, modelA, empty_position, sizeof(modelA) / sizeof(int));
//				if (sameCode) {
//					if (board[empty_position[1]][empty_position[0]] == 0) {
//						position[0] = empty_position[0];
//						position[1] = empty_position[1];
//						return 1;
//					}
//				}
//			}
//		}
//	}
//	//Check Alone Three
//	int modelB[5] = { 0, 1, 1, 1, 0 };
//	for (int i = 0; i < BOARD_SIZE; i++) {
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			if (atk_board[i][j] == 0) {
//				int positionTemp[2] = { i, j };
//				int sameCode = checkSameModel(atk_board, def_board, positionTemp, modelB, empty_position, sizeof(modelB) / sizeof(int));
//				if (sameCode) {
//					if (board[empty_position[1]][empty_position[0]] == 0) {
//						position[1] = i;
//						position[0] = j;
//						return 1;
//					}
//				}
//			}
//		}
//	}
//	for (int i = 0; i < BOARD_SIZE; i++) {
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			if (def_board[i][j] == 0) {
//				int positionTemp[2] = { i, j };
//				int sameCode = checkSameModel(def_board, atk_board, positionTemp, modelB, empty_position, sizeof(modelB) / sizeof(int));
//				if (sameCode) {
//					if (board[empty_position[1]][empty_position[0]] == 0) {
//						position[1] = i;
//						position[0] = j;
//						return 1;
//					}
//				}
//			}
//		}
//	}
//	return 0;
//}
//
//int checkSameModel(int target_board[][BOARD_SIZE], int opposite_board[][BOARD_SIZE], int position[], int model[], int empty_pos[], int size) {
//	//Return 1 : Horizon 2 : Verical 3 : SOUTH EAST 4 : SOUTH WEST
//	//1
//	int cnt = 0;
//	for (int i = 0; i < size; i++) {
//		if (position[1] + i < BOARD_SIZE) {
//			if (target_board[position[0]][position[1] + i] == model[i]) {
//				if (i != 0 && i != (size - 1) && model[i] == 0) {
//					empty_pos[1] = position[0];
//					empty_pos[0] = position[1] + i;
//				}
//				cnt++;
//			} else {
//				break;
//			}
//		} else {
//			break;
//		}
//	}
//	if (cnt == size && (opposite_board[position[0]][position[1]] == 0 && opposite_board[position[0]][position[1] + (size - 1)] == 0)) {
//		return 1;
//	}
//
//	cnt = 0;
//	for (int i = 0; i < size; i++) {
//		if (position[1] + i < BOARD_SIZE) {
//			if (target_board[position[0]][position[1] + i] == model[(size - 1) - i]) {
//				if (i != 0 && i != (size - 1) && model[(size - 1) - i] == 0) {
//					empty_pos[1] = position[0];
//					empty_pos[0] = position[1] + i;
//				}
//				cnt++;
//			} else {
//				break;
//			}
//		} else {
//			break;
//		}
//	}
//	if (cnt == size && (opposite_board[position[0]][position[1]] == 0 && opposite_board[position[0]][position[1] + (size - 1)] == 0)) {
//		return 1;
//	}
//	//2
//	cnt = 0;
//	for (int i = 0; i < size; i++) {
//		if (position[0] + i < BOARD_SIZE) {
//			if (target_board[position[0] + i][position[1]] == model[i]) {
//				if (i != 0 && i != (size - 1) && model[i] == 0) {
//					empty_pos[1] = position[0] + i;
//					empty_pos[0] = position[1];
//				}
//				cnt++;
//			} else {
//				break;
//			}
//		} else {
//			break;
//		}
//	}
//	if (cnt == size && (opposite_board[position[0]][position[1]] == 0 && opposite_board[position[0] + (size - 1)][position[1]] == 0)) {
//		return 2;
//	}
//	cnt = 0;
//	for (int i = 0; i < size; i++) {
//		if (position[0] + i < BOARD_SIZE) {
//			if (target_board[position[0] + i][position[1]] == model[size - 1 - i]) {
//				if (i != 0 && i != (size - 1) && model[(size - 1) - i] == 0) {
//					empty_pos[1] = position[0] + i;
//					empty_pos[0] = position[1];
//				}
//				cnt++;
//			} else {
//				break;
//			}
//		} else {
//			break;
//		}
//	}
//	if (cnt == size && (opposite_board[position[0]][position[1]] == 0 && opposite_board[position[0] + (size - 1)][position[1]] == 0)) {
//		return 2;
//	}
//
//	//3
//	cnt = 0;
//	for (int i = 0; i < size; i++) {
//		if (position[0] + i < BOARD_SIZE && position[1] + i < BOARD_SIZE) {
//			if (target_board[position[0] + i][position[1] + i] == model[i]) {
//				if (i != 0 && i != (size - 1) && model[i] == 0) {
//					empty_pos[1] = position[0] + i;
//					empty_pos[0] = position[1] + i;
//				}
//				cnt++;
//			} else {
//				break;
//			}
//		} else {
//			break;
//		}
//	}
//	if (cnt == size && (opposite_board[position[0]][position[1]] == 0 && opposite_board[position[0] + (size - 1)][position[1] + (size - 1)] == 0)) {
//		return 3;
//	}
//
//	cnt = 0;
//	for (int i = 0; i < size; i++) {
//		if (position[0] + i < BOARD_SIZE && position[1] + i < BOARD_SIZE) {
//			if (target_board[position[0] + i][position[1] + i] == model[size - 1 - i]) {
//				if (i != 0 && i != (size - 1) && model[(size - 1) - i] == 0) {
//					empty_pos[1] = position[0] + i;
//					empty_pos[0] = position[1] + i;
//				}
//				cnt++;
//			} else {
//				break;
//			}
//		} else {
//			break;
//		}
//	}
//	if (cnt == size && (opposite_board[position[0]][position[1]] == 0 && opposite_board[position[0] + (size - 1)][position[1] + (size - 1)] == 0)) {
//		return 3;
//	}
//
//	//4
//	cnt = 0;
//	for (int i = 0; i < size; i++) {
//		if (position[0] + i < BOARD_SIZE && position[1] - i > -1) {
//			if (target_board[position[0] + i][position[1] - i] == model[i]) {
//				if (i != 0 && i != (size - 1) && model[i] == 0) {
//					empty_pos[1] = position[0] + i;
//					empty_pos[0] = position[1] - i;
//				}
//				cnt++;
//			} else {
//				break;
//			}
//		} else {
//			break;
//		}
//	}
//	if (cnt == size && (opposite_board[position[0]][position[1]] == 0 && opposite_board[position[0] + (size - 1)][position[1] - (size - 1)] == 0)) {
//		return 4;
//	}
//
//	cnt = 0;
//	for (int i = 0; i < size; i++) {
//		if (position[0] + i < BOARD_SIZE && position[1] - i > -1) {
//			if (target_board[position[0] + i][position[1] - i] == model[size - 1 - i]) {
//				if (i != 0 && i != (size - 1) && model[(size - 1) - i] == 0) {
//					empty_pos[1] = position[0] + i;
//					empty_pos[0] = position[1] - i;
//				}
//				cnt++;
//			} else {
//				break;
//			}
//		} else {
//			break;
//		}
//	}
//	if (cnt == size && (opposite_board[position[0]][position[1]] == 0 && opposite_board[position[0] + (size - 1)][position[1] - (size - 1)] == 0)) {
//		return 4;
//	}
//
//	return 0;
//}
//
//int getBestPosition(int score_arr[][BOARD_SIZE], int atk_score_board[][BOARD_SIZE], int def_score_board[][BOARD_SIZE], int position[2]) {
//	//STEP 1
//	int position_stamp[BOARD_SIZE * BOARD_SIZE][2] = {0}, max = 0, cntA = 0;
//	for (int i = 0; i < BOARD_SIZE; i++) {
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			if (score_arr[i][j] > max) {
//				cntA = 1;
//				max = score_arr[i][j];
//				position_stamp[cntA - 1][0] = i;
//				position_stamp[cntA - 1][1] = j;
//			} else if (score_arr[i][j] == max) {
//				cntA++;
//				position_stamp[cntA - 1][0] = i;
//				position_stamp[cntA - 1][1] = j;
//			}
//		}
//	}
//	if (cntA == 1) {
//		
//		position[1] = position_stamp[0][0];
//		position[0] = position_stamp[0][1];
//		return 1;
//	
//	}
//
//	//STEP 2
//	int cntB = 0;
//	for (int i = 0; i < cntA; i++) {
//		score_arr[position_stamp[i][0]][position_stamp[i][1]] = score_arr[position_stamp[i][0]][position_stamp[i][1]] + atk_score_board[position_stamp[i][0]][position_stamp[i][1]];
//		if (score_arr[position_stamp[i][0]][position_stamp[i][1]] > max) {
//			cntB = 1;
//			max = score_arr[position_stamp[i][0]][position_stamp[i][1]];
//			position_stamp[cntB - 1][0] = position_stamp[i][0];
//			position_stamp[cntB - 1][1] = position_stamp[i][1];
//		} else if (score_arr[position_stamp[i][0]][position_stamp[i][1]] == max) {
//			cntB++;
//			position_stamp[cntB - 1][0] = position_stamp[i][0];
//			position_stamp[cntB - 1][1] = position_stamp[i][1];
//		}
//	}
//
//	if (cntB == 1) {
//		position[1] = position_stamp[0][0];
//		position[0] = position_stamp[0][1];
//		return 1;
//	}
//
//	//STEP 3
//	int cntC = 0;
//	for (int i = 0; i < cntB; i++) {
//		(score_arr[position_stamp[i][0]][position_stamp[i][1]] = score_arr[position_stamp[i][0]][position_stamp[i][1]] + atk_score_board[position_stamp[i][0]][position_stamp[i][1]]);
//		if (score_arr[position_stamp[i][0]][position_stamp[i][1]] > max) {
//			cntC = 1;
//			max = score_arr[position_stamp[i][0]][position_stamp[i][1]];
//			position_stamp[cntC - 1][0] = position_stamp[i][0];
//			position_stamp[cntC - 1][1] = position_stamp[i][1];
//		}
//		else if (score_arr[position_stamp[i][0]][position_stamp[i][1]] == max) {
//			cntC++;
//			position_stamp[cntC - 1][0] = position_stamp[i][0];
//			position_stamp[cntC - 1][1] = position_stamp[i][1];
//		}
//	}
//
//	if (cntC == 1) {
//		position[1] = position_stamp[0][0];
//		position[0] = position_stamp[0][1];
//		return 1;
//	}
//
//	//STEP 4
//	int randomValue = getRandomInt(cntC);
//	position[1] = position_stamp[randomValue - 1][0];
//	position[0] = position_stamp[randomValue - 1][1];
//
//	return 0;
//
//}
//
//void filterDisable(int target[][BOARD_SIZE]) {
//	for (int i = 0; i < BOARD_SIZE; i++) {
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			if (now_board[i][j] != 0) {
//				target[i][j] = -1;
//			}
//		}
//	}
//}
//
//
//void sumArray(int arrA[][BOARD_SIZE],int arrB[][BOARD_SIZE], int result[][BOARD_SIZE]) {
//	for (int i = 0; i < BOARD_SIZE; i++) {
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			result[i][j] = arrA[i][j] + arrB[i][j];
//		}
//	}
//}
//
//void extractBoard(int original_board[][BOARD_SIZE], int color, int extracted_board[][BOARD_SIZE]) {
//	for (int i = 0; i < BOARD_SIZE; i++) {
//		for (int j = 0; j < BOARD_SIZE; j++) {
//			if (original_board[i][j] == color) {
//				extracted_board[i][j] = 1; //Exist
//			}
//		}
//	}
//}
//
//
//void continueousScore(int board[][BOARD_SIZE], int return_board[][BOARD_SIZE]) {
//	int boardTemp[BOARD_SIZE + 2][BOARD_SIZE + 2] = { 0 };
//	int result_larger_board[BOARD_SIZE + 2][BOARD_SIZE + 2] = { 0 };
//	for (int i = 1; i < BOARD_SIZE + 1; i++) {
//		for (int j = 1; j < BOARD_SIZE + 1; j++) {
//			boardTemp[i][j] = board[i - 1][j - 1];
//		}
//	}
//
//	for (int i = 1; i < BOARD_SIZE + 1; i++) {
//		for (int j = 1; j < BOARD_SIZE + 1; j++) {
//			
//			if (boardTemp[j][i] == 1) {
//				int debug_j = j;
//				//Horizon
//				int cnt_plus = 0, cnt_minus = 0, k = 0;
//				while (1) {
//					int debug_j = j;
//					k++;
//					if (boardTemp[j][i + k] == 1) {
//						cnt_plus++;
//					} else {
//						break;
//					}
//				}
//				k = 0;
//				while (1) {
//					k--;
//					if (boardTemp[j][i + k] == 1) {
//						cnt_minus++;
//					} else {
//						break;
//					}
//				}
//
//				int cnt_total = cnt_plus + cnt_minus + 1;
//				if (cnt_total > result_larger_board[j][i + (cnt_plus + 1)]) {
//					result_larger_board[j][i + (cnt_plus + 1)] = cnt_total;
//				}
//				if (cnt_total > result_larger_board[j][i - (cnt_minus + 1)]) {
//					result_larger_board[j][i - (cnt_minus + 1)] = cnt_total;
//				}
//
//				//Vertical
//				cnt_plus = 0, cnt_minus = 0, k = 0;
//				while (1) {
//					k++;
//					if (boardTemp[j + k][i] == 1) {
//						cnt_plus++;
//					} else {
//						break;
//					}
//				}
//				k = 0;
//				while (1) {
//					k--;
//					if (boardTemp[j + k][i] == 1) {
//						cnt_minus++;
//					} else {
//						break;
//					}
//				}
//				cnt_total = cnt_plus + cnt_minus + 1;
//				if (cnt_total > result_larger_board[j + (cnt_plus + 1)][i]) {
//					result_larger_board[j + (cnt_plus + 1)][i] = cnt_total;
//				}
//				if (cnt_total > result_larger_board[j - (cnt_minus + 1)][i]) {
//					result_larger_board[j - (cnt_minus + 1)][i] = cnt_total;
//				}
//				
//				//EAST NORTH
//				cnt_plus = 0, cnt_minus = 0, k = 0;
//				while (1) {
//					k++;
//					if (boardTemp[j + k][i + k] == 1) {
//						cnt_plus++;
//					} else {
//						break;
//					}
//				}
//				k = 0;
//				while (1) {
//					k--;
//					if (boardTemp[j + k][i + k] == 1) {
//						cnt_minus++;
//					} else {
//						break;
//					}
//				}
//				cnt_total = cnt_plus + cnt_minus + 1;
//				if (cnt_total > result_larger_board[j + (cnt_plus + 1)][i + (cnt_plus + 1)]) {
//					result_larger_board[j + (cnt_plus + 1)][i + (cnt_plus + 1)] = cnt_total;
//				}
//				if (cnt_total > result_larger_board[j - (cnt_minus + 1)][i - (cnt_minus + 1)]) {
//					result_larger_board[j - (cnt_minus + 1)][i - (cnt_minus + 1)] = cnt_total;
//				}
//
//				//EAST SOUTH
//				cnt_plus = 0, cnt_minus = 0, k = 0;
//				while (1) {
//					k++;
//					if (boardTemp[j + k][i - k] == 1) {
//						cnt_plus++;
//					} else {
//						break;
//					}
//				}
//				k = 0;
//				while (1) {
//					k--;
//					if (boardTemp[j + k][i - k] == 1) {
//						cnt_minus++;
//					} else {
//						break;
//					}
//				}
//				cnt_total = cnt_plus + cnt_minus + 1;
//				if (cnt_total > result_larger_board[j + (cnt_plus + 1)][i - (cnt_plus + 1)]) {
//					result_larger_board[j + (cnt_plus + 1)][i - (cnt_plus + 1)] = cnt_total;
//				}
//				if (cnt_total > result_larger_board[j - (cnt_minus + 1)][i + (cnt_minus + 1)]) {
//					result_larger_board[j - (cnt_minus + 1)][i + (cnt_minus + 1)] = cnt_total;
//				}
//			}
//		}
//	}
//
//	for (int i = 1; i < BOARD_SIZE + 1; i++) {
//		for (int j = 1; j < BOARD_SIZE + 1; j++) {
//			return_board[i - 1][j - 1] = result_larger_board[i][j];
//		}
//	}
//}
//
//
//int checkCapability(int position[]) {
//	if (now_board[position[1]][position[0]] != 0) {
//		return 0;
//	} else {
//		return 1;
//	}
//}