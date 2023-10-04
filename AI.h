#pragma once
#include "GameBrain.h"
class AI  : public GameBrain{
public:
	void make_move() override;
	AI(Player *p1, Player* p2, Board* board, ScoreBoard* score_board);
private:
	int random_number(int max);
};


