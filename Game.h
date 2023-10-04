#pragma once
#include<string>
#include"Board.h"
#include"Player.h"
#include"GameBrain.h"

using namespace std;

class Game
{
private:
	Board* board;
	GameBrain* game_brain;
	int game_options_number = 1;
public:
	void game();
private:
	string get_user_nick(string msg);
	void greet_players(Player* p1, Player* p2);
	void set_board(Board* board);
	void set_game_brain(GameBrain* game_brain);
	int get_menu_choice();
	bool validate_nick(string nick);
};

