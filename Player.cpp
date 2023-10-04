#include "Player.h"
#include<string>

using namespace std;

Player::Player(string nick, char color){
	this->nick = nick;
	this->color = color;
}

string Player::get_nick() {
	return this->nick;
}

string Player::get_full_pawn_color() {
	if (this->color == 'C') return "czarny";
	if (this->color == 'B') return "bialy";
}

char Player::get_letter_pawn_color() {
	return this->color;
}

bool Player::get_has_surrendered() { return has_surrendered; }

void Player::set_has_surrendered(bool has_surrendered) { this->has_surrendered = has_surrendered; }

