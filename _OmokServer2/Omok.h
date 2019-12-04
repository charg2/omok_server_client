#pragma once

class Omok 
{
private:
	// 현재 오목을 둘 플레이어 선택을 위한 변수
	OmokStatus current_turn;

	// 오목 승자 선택을 위한 변수
	OmokStatus winner;
	
	// 게임판.
	tagGomokuInfo	gomoku[19][19];
	bool			is_over;

public:
	Omok();
	~Omok();

	bool init();
	void fin();
	void update() ;
	void update_network();

	bool is_gameover();

	void CheckGomoku(int row, int col, OmokStatus current);
	void GameOver(OmokStatus winner);
};

extern Omok* g_omok;