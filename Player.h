#pragma once
#include<string>
#include<iostream>

using namespace std;

class Player
{
private:
	string nick;
	char color;
	bool has_surrendered = false;
public:
	Player(string nick, char color);
	string get_nick();
	string get_full_pawn_color();
	char get_letter_pawn_color();
	bool get_has_surrendered();
	void set_has_surrendered(bool has_surrendered);
};

