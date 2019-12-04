#include "stdafx.h"
#include "OmokScene.h"
#include "OmokClient.h" 


OmokScene::OmokScene()
{
	this->client = new OmokClient;
	this->client->set_game(this);

	game_time.reserve(100);
}
OmokScene::~OmokScene()
{
	delete this->client;
}

HRESULT OmokScene::Init()
{
	map = new Image;
	map->Init("images/gomoku/map.bmp", 19 * 25 + 10, 19 * 25 + 10);

	black = new Image;
	black->Init("images/gomoku/black.bmp", 25, 25, true, RGB(240, 191, 112));

	white = new Image;
	white->Init("images/gomoku/white.bmp", 25, 25, true, RGB(240, 191, 112));
	
	menu = new Image;
	menu->Init("images/gomoku/menu.bmp", 19 * 25 + 10, 25, true, RGB(240, 191, 112));

	for (int i = 0; i < 19; i++) 
	{
		for (int j = 0; j < 19; j++) 
		{
			gomoku[i][j].status = OMOK_EMPTY;
			gomoku[i][j].x = 466 + i * 25;
			gomoku[i][j].y = 102 + j * 25;
			gomoku[i][j].rc = RectMake(gomoku[i][j].x, gomoku[i][j].y,
				25, 25);
		}
	}
	// �浹���� �����̶� ���� ���� ���� �浹�� ����
	current_turn = OMOK_BLACK;
	myOmokStone = OMOK_EMPTY; // �������� ��������.
	isOver = false;

	rcGomokuExit = RectMake(923, 75, 25, 25);

	if (false == client->init())
	{
		err_quit("OmokScene::client->init() failure");
		err_display("OmokScene::client->init() failure");
		
		return S_FALSE;
	}


	client->connect();

	this->time_tick = 0;
	return S_OK;
}

void OmokScene::Release()
{
	SAFE_DELETE(map);
	SAFE_DELETE(black);
	SAFE_DELETE(white);
	SAFE_DELETE(menu);
	delete this->client;
}

void OmokScene::Update()
{
	// �� �����ϋ��� �������� üũ �ƴϸ� ���� �ƿ� 
	if (this->current_turn == this->myOmokStone)
	{
		if (INPUT->GetKeyDown(VK_LBUTTON))
		{
			// ���� ��ư Ŭ���� �� ����
			if (PtInRect(&rcGomokuExit, g_ptMouse))
			{
				SCENE->ChangeScene("None");
				return;
			}

			// �̰� ���� �ִ°� �´°� �����غ���.
			//if (isOver)
			//{
			//	Init();
			//	return;
			//}

			for (int i = 0; i < 19; i++)
			{
				for (int j = 0; j < 19; j++)
				{
					// ������ �� ��ġ�� ���콺 Ŭ���� 
					// ��ǥ�� 
					if (PtInRect(&gomoku[i][j].rc, g_ptMouse) && gomoku[i][j].status == OMOK_EMPTY)
					{
						// ij ��ǥ�� ��û�� ����. �� �� ���¸� ���� 
						client->requset_to_drop_stone(i, j, this->myOmokStone);

						// ���� ������ ���� Ŭ��
						//gomoku[i][j].status = currentOmoku;

						//// ���� �ΰ� ���� ���� ���� ���� ����
						//this->ChangeTurn();
						//if (currentOmoku == GOMOKU_BLACK)
						//	currentOmoku = GOMOKU_WHITE;
						//else if (currentOmoku == GOMOKU_WHITE)
						//	currentOmoku = GOMOKU_BLACK;

						//���� �ΰ� ���� �¸� üũ
						//CheckGomoku(i, j, gomoku[i][j].status);
					}
				}
			}
		}
	}
//====================== Netwrk Udate ==============//
	client->update();
	// �ް� ��Ŷ ó�� �� ������ ��Ŷ ó��.
//==================================================//

//====================== Debug =====================//
	if (INPUT->GetKeyDown(VK_TAB)) 
	{
		isDebug = !isDebug;
	}
//==================================================//
}


#include <string>
void OmokScene::Render()
{
	menu->Render(GetMemDC(), WINSIZEX / 2 - 50, 100 - 25);

	map->Render(GetMemDC(), WINSIZEX/2 - 50, 100);

	// ���¿� ���� ���� �׸���
	for (int i = 0; i < 19; i++) 
	{
		for (int j = 0; j < 19; j++) 
		{
			switch (gomoku[i][j].status)
			{
			case OMOK_EMPTY:
				break;
			case OMOK_BLACK:
				black->Render(GetMemDC(), gomoku[i][j].x, gomoku[i][j].y);
				break;
			case OMOK_WHITE:
				white->Render(GetMemDC(), gomoku[i][j].x, gomoku[i][j].y);
				break;
			}
		}
	}

	// test
	//black->Render(GetMemDC());
	//white->Render(GetMemDC(),25, 0);

	HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("�ü�"));
	HFONT OldFont = (HFONT)SelectObject(GetMemDC(), hFont);

	// ���� �ð� �׸���.
	const static std::string timer_format = "TIME : ";

	std::string current_time_tick = timer_format + std::to_string( this->time_tick / 1000 );

	TextOutA(GetMemDC(), 568, 10, current_time_tick.c_str(), current_time_tick.size());

	if (isOver) 
	{
		switch (winner)
		{
		//case OMOK_EMPTY:
		//	sprintf_s(str, "    DRAW");
		//	break;
		case OMOK_BLACK:
			sprintf_s(str, "BLACK WIN");
			break;
		case OMOK_WHITE:
			sprintf_s(str, "WHITE WIN");
			break;
		}

		TextOutA(GetMemDC(), 568, 273, str, strlen(str));

		//sprintf_s(str, "Any Click");
		//TextOutA(GetMemDC(), 575, 353, str, strlen(str));

		//sprintf_s(str, "ReStart");
		//TextOutA(GetMemDC(), 600, 403, str, strlen(str));
	}

	SelectObject(GetMemDC(), OldFont);
	DeleteObject(hFont);

	////==================   Debug   ====================
	//if (isDebug)
	//{
	//	for (int i = 0; i < 19; i++) {
	//		for (int j = 0; j < 19; j++) {
	//			RectangleMake(GetMemDC(), gomoku[i][j].rc);
	//		}
	//	}
	//	RectangleMake(GetMemDC(), rcGomokuExit);
	//}
	////=================================================
}

// ���� ���� üũ
void OmokScene::CheckGomoku(int row, int col, OmokStatus current)
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

void OmokScene::GameOver(OmokStatus winner)
{
	this->winner = winner;
	isOver = true;
}

void OmokScene::SetMyOmokStone(OmokStatus stone)
{
	this->myOmokStone = stone;
}

OmokStatus OmokScene::GetOmokStone()
{
	return this->myOmokStone;
}

void OmokScene::SwapTurn()
{
	// ���� �ΰ� ���� ���� ���� ���� ����
	if (current_turn == OMOK_BLACK)
		current_turn = OMOK_WHITE;
	else if (current_turn == OMOK_WHITE)
		current_turn = OMOK_BLACK;
}

void OmokScene::DropStone(int x, int y, OmokStatus stone)
{
	gomoku[x][y].status = stone;
}

void OmokScene::SetTimeTick(size_t tick)
{
	this->time_tick = tick;
}
