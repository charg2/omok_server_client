#include "stdafx.h"
#include "../../Common/common.h"
#include "Omok.h"


Omok::Omok() : game_status{ GAME_BEFORE }, is_over{ false }, current_turn{OMOK_BLACK}, winner{ OMOK_EMPTY }
{
}

Omok::~Omok()
{
}

bool Omok::Init()
{

	for (int i = 0; i < 19; i++) 
	{
		for (int j = 0; j < 19; j++) 
		{
			// test
			//gomoku[i][j].status = OMOK_BLACK;
			gomoku[i][j].status = OMOK_EMPTY;
			//gomoku[i][j].x = 466 + i * 25;
			//gomoku[i][j].y = 102 + j * 25;

		}
	}

	// 흑돌부터 시작이라 현재 오목 상태 흑돌로 변경
	current_turn = OMOK_BLACK;
	game_status = GAME_BEFORE;

	is_over = false;

	return S_OK;
}

void Omok::Fin()
{
}



void Omok::Update()
{
	// 데이터 받고 

	//printf("OMOK::UPDATE()");

	// 게임 종료 조건이 트루이면 각 클라이언트에게 정보를보내줌 게임이 끝나고 누가 승리한것인지.
	//if (is_over) 
	//{
	//	// notifyResult();
	//	init();
	//	return;
	//}

	//int x = 0, y = 0;
	//// 오목의 빈 위치에 마우스 클릭시

	//if (gomoku[x][y].status == OMOK_EMPTY)
	//{
	//	// 현재 차례의 오목 클릭
	//	gomoku[x][y].status = current_turn;

	//	// 오목 두고 난후 현재 오목 상태 변경
	//	if (current_turn == OMOK_BLACK)
	//		current_turn = OMOK_WHITE;
	//	else if (current_turn == OMOK_WHITE)
	//		current_turn = OMOK_BLACK;
	//}
	//else
	//{
	//	// 반영안하고 종료 
	//	// 
	//}

//	// 한명이 둘떄마다 체크.
//	// update 이후 체크.. 승리자가 있는지.
//	for (int i = 0; i < 19; i++) 
//	{
//		for (int j = 0; j < 19; j++) 
//		{
//			// 넘겨준 좌표 ij가 
//			//if (gomoku[i][j].status == OMOK_EMPTY) 
//			//{
//			//오목 두고 난후 승리 체크
//				CheckGomoku(i,j, gomoku[i][j].status);
//			//}
//		}
//	}
}


bool Omok::IsGameover()
{
	return this->is_over;
}

bool Omok::DropStone(int x, int j, OmokStatus type)
{
	if (OMOK_EMPTY == gomoku[x][j].status)
	{
		gomoku[x][j].status = type;
		return true;
	}
	else
		return false;
}

// 오목 상태 체크
void Omok::CheckGomoku(int row, int col, OmokStatus current)
{
	// 시작점
	int x, y;

	int count;

	// 가로 체크
	x = row;
	y = col;
	count = 0;

	// 가로 시작 위치 찾기
	while (gomoku[x - 1][y].status == current && x > 0) x--;

	// 가로 시작 위치부터 해당 돌과 같은 오목 갯수 세기
	while (gomoku[x++][y].status == current && x <= 18) count++;

	// 오목이면 해당 차례의 돌의 승리로 게임 종료
	if (count == 5)
	{
		printf("승자 발생");
		GameOver(current);
		return;
	}
	// 세로 체크
	x = row;
	y = col;
	count = 0;

	// 세로 시작 위치 찾기
	while (gomoku[x][y - 1].status == current && y > 0) y--;

	// 세로 시작 위치부터 해당 돌과 같은 오목 갯수 세기
	while (gomoku[x][y++].status == current && y <= 18) count++;

	if (count == 5)
	{
		printf("승자 발생");
		GameOver(current);
		return;
	}
	// 대각선 체크
	x = row;
	y = col;
	count = 0;

	// 대각선 시작 위치 찾기
	while (gomoku[x - 1][y - 1].status == current
		&& x > 0 && y > 0) {
		x--;
		y--;
	}

	// 대각선 시작 위치부터 해당 돌과 같은 오목 갯수 세기
	while (gomoku[x++][y++].status == current
		&& x <= 18 && y <= 18) count++;

	if (count == 5)
	{
		printf("승자 발생");
		GameOver(current);
		return;
	}

	// 역대각선 체크
	x = row;
	y = col;
	count = 0;

	// 역대각선 시작 위치 찾기
	while (gomoku[x - 1][y + 1].status == current
		&& x > 0 && y < 18) {
		x--;
		y++;
	}

	// 역대각선 시작 위치부터 해당 돌과 같은 오목 갯수 세기
	while (gomoku[x++][y--].status == current
		&& x <= 18 && y >= 0) count++;

	if (count == 5)
	{
		printf("승자 발생");
		GameOver(current);
		return;
	}


}

void Omok::GameOver(OmokStatus winner)
{
	this->winner = winner;

	is_over = true;

	// 패킷을 보냄 둠여다한테 이긴 사람 진 사람 다.
	// 니가 짐 
	// 니가 이김.

	// 상대방이 이김.
}

void Omok::CheckGame()
{
	// 한명이 둘떄마다 체크.
	// update 이후 체크.. 승리자가 있는지.
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			// 넘겨준 좌표 ij가 
			//if (gomoku[i][j].status == OMOK_EMPTY) 
			//{
			//오목 두고 난후 승리 체크
			CheckGomoku(i, j, gomoku[i][j].status);
			//}
		}
	}
}

void Omok::SwapTurn()
{
	// 오목 두고 난후 현재 오목 상태 변경
	if (current_turn == OMOK_BLACK)
		current_turn = OMOK_WHITE;
	else if (current_turn == OMOK_WHITE)
		current_turn = OMOK_BLACK;
}

OmokStatus Omok::GetTurn()
{
	return this->current_turn;
}

OmokStatus Omok::GetWinner()
{
	return this->winner;
}
