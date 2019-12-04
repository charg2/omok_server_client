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

	// �浹���� �����̶� ���� ���� ���� �浹�� ����
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
	// ������ �ް� 

	//printf("OMOK::UPDATE()");

	// ���� ���� ������ Ʈ���̸� �� Ŭ���̾�Ʈ���� ������������ ������ ������ ���� �¸��Ѱ�����.
	//if (is_over) 
	//{
	//	// notifyResult();
	//	init();
	//	return;
	//}

	//int x = 0, y = 0;
	//// ������ �� ��ġ�� ���콺 Ŭ����

	//if (gomoku[x][y].status == OMOK_EMPTY)
	//{
	//	// ���� ������ ���� Ŭ��
	//	gomoku[x][y].status = current_turn;

	//	// ���� �ΰ� ���� ���� ���� ���� ����
	//	if (current_turn == OMOK_BLACK)
	//		current_turn = OMOK_WHITE;
	//	else if (current_turn == OMOK_WHITE)
	//		current_turn = OMOK_BLACK;
	//}
	//else
	//{
	//	// �ݿ����ϰ� ���� 
	//	// 
	//}

//	// �Ѹ��� �ы����� üũ.
//	// update ���� üũ.. �¸��ڰ� �ִ���.
//	for (int i = 0; i < 19; i++) 
//	{
//		for (int j = 0; j < 19; j++) 
//		{
//			// �Ѱ��� ��ǥ ij�� 
//			//if (gomoku[i][j].status == OMOK_EMPTY) 
//			//{
//			//���� �ΰ� ���� �¸� üũ
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

// ���� ���� üũ
void Omok::CheckGomoku(int row, int col, OmokStatus current)
{
	// ������
	int x, y;

	int count;

	// ���� üũ
	x = row;
	y = col;
	count = 0;

	// ���� ���� ��ġ ã��
	while (gomoku[x - 1][y].status == current && x > 0) x--;

	// ���� ���� ��ġ���� �ش� ���� ���� ���� ���� ����
	while (gomoku[x++][y].status == current && x <= 18) count++;

	// �����̸� �ش� ������ ���� �¸��� ���� ����
	if (count == 5)
	{
		printf("���� �߻�");
		GameOver(current);
		return;
	}
	// ���� üũ
	x = row;
	y = col;
	count = 0;

	// ���� ���� ��ġ ã��
	while (gomoku[x][y - 1].status == current && y > 0) y--;

	// ���� ���� ��ġ���� �ش� ���� ���� ���� ���� ����
	while (gomoku[x][y++].status == current && y <= 18) count++;

	if (count == 5)
	{
		printf("���� �߻�");
		GameOver(current);
		return;
	}
	// �밢�� üũ
	x = row;
	y = col;
	count = 0;

	// �밢�� ���� ��ġ ã��
	while (gomoku[x - 1][y - 1].status == current
		&& x > 0 && y > 0) {
		x--;
		y--;
	}

	// �밢�� ���� ��ġ���� �ش� ���� ���� ���� ���� ����
	while (gomoku[x++][y++].status == current
		&& x <= 18 && y <= 18) count++;

	if (count == 5)
	{
		printf("���� �߻�");
		GameOver(current);
		return;
	}

	// ���밢�� üũ
	x = row;
	y = col;
	count = 0;

	// ���밢�� ���� ��ġ ã��
	while (gomoku[x - 1][y + 1].status == current
		&& x > 0 && y < 18) {
		x--;
		y++;
	}

	// ���밢�� ���� ��ġ���� �ش� ���� ���� ���� ���� ����
	while (gomoku[x++][y--].status == current
		&& x <= 18 && y >= 0) count++;

	if (count == 5)
	{
		printf("���� �߻�");
		GameOver(current);
		return;
	}


}

void Omok::GameOver(OmokStatus winner)
{
	this->winner = winner;

	is_over = true;

	// ��Ŷ�� ���� �ҿ������� �̱� ��� �� ��� ��.
	// �ϰ� �� 
	// �ϰ� �̱�.

	// ������ �̱�.
}

void Omok::CheckGame()
{
	// �Ѹ��� �ы����� üũ.
	// update ���� üũ.. �¸��ڰ� �ִ���.
	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			// �Ѱ��� ��ǥ ij�� 
			//if (gomoku[i][j].status == OMOK_EMPTY) 
			//{
			//���� �ΰ� ���� �¸� üũ
			CheckGomoku(i, j, gomoku[i][j].status);
			//}
		}
	}
}

void Omok::SwapTurn()
{
	// ���� �ΰ� ���� ���� ���� ���� ����
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
