#pragma once

class Omok 
{
public:
	Omok();
	~Omok();

	bool Init();
	void Fin();
	void Update() ;

	bool IsGameover();

	bool DropStone(int x, int y, OmokStatus type);

	void CheckGomoku(int row, int col, OmokStatus current);
	void GameOver(OmokStatus winner);

	void CheckGame();
	void SwapTurn();
	OmokStatus GetTurn();
	OmokStatus GetWinner();
private:

	// 현재 오목을 둘 플레이어 선택을 위한 변수
	OmokStatus current_turn;

	// 오목 승자 선택을 위한 변수
	OmokStatus winner;
	
	// 게임판.
	tagGomokuInfo	gomoku[19][19];
	bool			is_over;

	GameStatus		game_status;
};

extern Omok* g_omok;