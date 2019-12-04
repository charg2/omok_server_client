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

	// ���� ������ �� �÷��̾� ������ ���� ����
	OmokStatus current_turn;

	// ���� ���� ������ ���� ����
	OmokStatus winner;
	
	// ������.
	tagGomokuInfo	gomoku[19][19];
	bool			is_over;

	GameStatus		game_status;
};

extern Omok* g_omok;