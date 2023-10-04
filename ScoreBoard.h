#pragma once
#include<string>
#include<fstream>
#include "Player.h"
	
using namespace std;

class ScoreBoard{
private:
	string file_name = "score.txt";
	ofstream file;
	ifstream read_file;
public:
	void add_game_score(Player* p1, Player* p2, int white_score, int black_score);
	void print_score_board();
};

