#pragma once

class Omok 
{
private:
	// ���� ������ �� �÷��̾� ������ ���� ����
	OmokStatus current_turn;

	// ���� ���� ������ ���� ����
	OmokStatus winner;
	
	// ������.
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