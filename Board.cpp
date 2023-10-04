#include "Board.h"
#include<iostream>
using namespace std;

void Board::print() {
	cout << endl;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < this->borad_size; j++) {
			cout << board[i][j];
		}
		if (i % 2 == 0) {
			cout << " " << i / 2;
		}
		cout << endl;
	}
	char start = 'a';
	for (int i = 0; i < this->movable_fields; i++) {
		cout << start++ << " ";
	} 
	cout << endl;
	cout << endl;
};

void Board::print_board_legend() {
	cout << endl;
	cout << "B - black pawns" << endl;
	cout << "W - white pawns" << endl;
	cout << "X - empty field" << endl;
}

char Board::get_board_element(int x, int y) {
	return board[x][y];
}

int Board::get_movable_fields() {
	return this->movable_fields;
}

char Board::get_last_letter() {
	return this->last_letter;
}

char Board::get_first_letter() {
	return this->first_letter;
}

void Board::set_board_element(int x, int y, char c) {
	this->board[x][y] = c;
}
