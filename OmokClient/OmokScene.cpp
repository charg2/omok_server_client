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
	// 흑돌부터 시작이라 현재 오목 상태 흑돌로 변경
	current_turn = OMOK_BLACK;
	myOmokStone = OMOK_EMPTY; // 서버에서 지정해줌.
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
	// 내 차례일떄만 눌렀는지 체크 아니면 무시 아예 
	if (this->current_turn == this->myOmokStone)
	{
		if (INPUT->GetKeyDown(VK_LBUTTON))
		{
			// 종료 버튼 클릭시 씬 끄기
			if (PtInRect(&rcGomokuExit, g_ptMouse))
			{
				SCENE->ChangeScene("None");
				return;
			}

			// 이게 여기 있는게 맞는가 생각해보자.
			//if (isOver)
			//{
			//	Init();
			//	return;
			//}

			for (int i = 0; i < 19; i++)
			{
				for (int j = 0; j < 19; j++)
				{
					// 오목의 빈 위치에 마우스 클릭시 
					// 좌표가 
					if (PtInRect(&gomoku[i][j].rc, g_ptMouse) && gomoku[i][j].status == OMOK_EMPTY)
					{
						// ij 좌표로 요청만 보냄. 내 돌 상태를 ㅇㅇ 
						client->requset_to_drop_stone(i, j, this->myOmokStone);

						// 현재 차례의 오목 클릭
						//gomoku[i][j].status = currentOmoku;

						//// 오목 두고 난후 현재 오목 상태 변경
						//this->ChangeTurn();
						//if (currentOmoku == GOMOKU_BLACK)
						//	currentOmoku = GOMOKU_WHITE;
						//else if (currentOmoku == GOMOKU_WHITE)
						//	currentOmoku = GOMOKU_BLACK;

						//오목 두고 난후 승리 체크
						//CheckGomoku(i, j, gomoku[i][j].status);
					}
				}
			}
		}
	}
//====================== Netwrk Udate ==============//
	client->update();
	// 받고 패킷 처리 및 보내기 패킷 처리.
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

	// 상태에 따라 오목 그리기
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
		VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
	HFONT OldFont = (HFONT)SelectObject(GetMemDC(), hFont);

	// 게임 시간 그리기.
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

// 오목 상태 체크
void OmokScene::CheckGomoku(int row, int col, OmokStatus current)
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
		GameOver(current);

	// 세로 체크
	x = row;
	y = col;
	count = 0;

	// 세로 시작 위치 찾기
	while (gomoku[x][y - 1].status == current && y > 0) y--;

	// 세로 시작 위치부터 해당 돌과 같은 오목 갯수 세기
	while (gomoku[x][y++].status == current && y <= 18) count++;

	if (count == 5)
		GameOver(current);

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
		GameOver(current);

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
	// 오목 두고 난후 현재 오목 상태 변경
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
