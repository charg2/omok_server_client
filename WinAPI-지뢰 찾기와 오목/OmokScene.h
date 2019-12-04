#pragma once
#include "../Common/const.h"
#include "GameNode.h"
#include "OmokClient.h" 

class OmokClient;
class OmokScene : public GameNode
{
public:
	OmokScene();
	~OmokScene();

	virtual HRESULT Init() override;
	virtual void Release() override;
	virtual void Update() override;
	virtual void Render() override;

	void CheckGomoku(int row, int col, OmokStatus current);
	void GameOver(OmokStatus winner);

	void SetMyOmokStone(OmokStatus stone);
	OmokStatus GetOmokStone();
	void SwapTurn();
	void DropStone(int x, int y, OmokStatus stone);
	void SetTimeTick(size_t tick);

private:
	bool isDebug;
	char str[128];

	std::string game_time;

	Image* map;
	Image* menu;
	Image* black;
	Image* white;

// 
	size_t			time_tick;
	// ���� ������ �� �÷��̾� ������ ���� ����
	OmokStatus		current_turn;

	// ���� ���� ������ ���� ����
	OmokStatus		winner;

	tagGomokuInfo	gomoku[19][19];
	bool			isOver;

	RECT			rcGomokuExit;
	OmokClient*		client;
// custom
	OmokStatus		myOmokStone;
};

