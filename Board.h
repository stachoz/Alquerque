#pragma once
#define BOARD_SIZE 9
class Board
{
private:
	int borad_size = BOARD_SIZE;
	int movable_fields = 5;
	char last_letter = 'e';
	char first_letter = 'a';
public:
	char board[BOARD_SIZE][BOARD_SIZE] = {
		{'C', '-','C','-','C','-','C','-', 'C'},			// 0
		{'|', '\\', '|', '/', '|', '\\', '|', '/', '|'},	// 1
		{'C', '-','C','-','C','-','C','-', 'C'},			// 2
		{'|', '/', '|', '\\', '|', '/', '|', '\\', '|'},	// 3
		{'B', '-','B','-','X','-','C','-', 'C'},			// 4
		{'|', '\\', '|', '/', '|', '\\', '|', '/', '|'},	// 5
		{'B', '-','B','-','B','-','B','-', 'B'},			// 6
		{'|', '/', '|', '\\', '|', '/', '|', '\\', '|'},	// 7 
		{'B', '-','B','-','B','-','B','-', 'B'},			// 8
	};
public:
	void print();
	void print_board_legend();
	char get_board_element(int x, int y);
	int get_movable_fields();
	char get_last_letter();
	char get_first_letter();
	void set_board_element(int x, int y, char c);
};

