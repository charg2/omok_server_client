#include "stdafx.h"
#include "../Common/common.h"
#include "Omok.h"


Omok::Omok()
{
}


Omok::~Omok()
{
}

bool Omok::init()
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

	is_over = false;

	return S_OK;
}

void Omok::fin()
{
}



void Omok::update()
{
	// ������ �ް� 



	// ���� ���� ������ Ʈ���̸� �� Ŭ���̾�Ʈ���� ������������ ������ ������ ���� �¸��Ѱ�����.
	if (is_over) 
	{
		// notifyResult();
		init();
		return;
	}

	int x, y;
	// ������ �� ��ġ�� ���콺 Ŭ����

	if (gomoku[x][y].status == OMOK_EMPTY)
	{
		// ���� ������ ���� Ŭ��
		gomoku[x][y].status = current_turn;

		// ���� �ΰ� ���� ���� ���� ���� ����
		if (current_turn == OMOK_BLACK)
			current_turn = OMOK_WHITE;
		else if (current_turn == OMOK_WHITE)
			current_turn = OMOK_BLACK;
	}
	else
	{
		// �ݿ����ϰ� ���� 
		// 
	}

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
				CheckGomoku(i,j, gomoku[i][j].status);
			//}
		}
	}
}

void Omok::update_network()
{
}



bool Omok::is_gameover()
{
	return this->is_over;
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
		GameOver(current);

	// ���� üũ
	x = row;
	y = col;
	count = 0;

	// ���� ���� ��ġ ã��
	while (gomoku[x][y - 1].status == current && y > 0) y--;

	// ���� ���� ��ġ���� �ش� ���� ���� ���� ���� ����
	while (gomoku[x][y++].status == current && y <= 18) count++;

	if (count == 5)
		GameOver(current);

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
		GameOver(current);

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
		GameOver(current);
}

void Omok::GameOver(OmokStatus winner)
{
	this->winner = winner;

	is_over = true;

	// ��Ŷ�� ���� �ҿ������� �̱� ��� �� ��� ��.
	// �ϰ� �� 
	// �ϰ� �̱�.
}
