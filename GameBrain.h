#pragma once
#include "Player.h"
#include "Board.h"
#include "ScoreBoard.h"
#include<vector>

struct coordinates {
	char letter;
	int number;
};

struct tab_2d_indexes {
	int x;
	int y;
};

class GameBrain
{
protected:
	static int move_counter; 
	static int white_pawns; // 12
	static int black_pawns;
	static ScoreBoard* score_board;
	static bool computer_move;
	static Board* board;
	bool are_there_moves(tab_2d_indexes pawn_co, char field_color);
	vector<tab_2d_indexes> find_all_captures(tab_2d_indexes pawn_co);
	vector<tab_2d_indexes> find_all_moves(tab_2d_indexes pawn_co);
	void capture(tab_2d_indexes pawn_co, tab_2d_indexes move_co);
	void move(tab_2d_indexes pawn_co, tab_2d_indexes move_co);
	coordinates convert(tab_2d_indexes co);
private:
	bool is_draw = false;
	int basic_pawns_num = 12;
	Player* player1;
	Player* player2;
	bool missed_capture = false;
	vector<tab_2d_indexes> pawns_to_take;
	void multi_capture_mechanism(tab_2d_indexes pawn_co);
	tab_2d_indexes convert(coordinates co);
	bool validate_pawn(char color, tab_2d_indexes pawn_co);
	bool validate_move(tab_2d_indexes pawn_co, tab_2d_indexes move_co);
	int take_number();
	char take_letter();
	char take_answer();
	bool can_player_move(char c);
	bool can_capture(char c);
	bool check_up(tab_2d_indexes pawn_co, char c);
	bool check_down(tab_2d_indexes pawn_co, char c);
	bool check_left(tab_2d_indexes pawn_co, char c);
	bool check_right(tab_2d_indexes pawn_co, char c);
	bool check_upper_right_bevel(tab_2d_indexes pawn_co, char c);
	bool check_upper_left_bevel(tab_2d_indexes pawn_co, char c);
	bool check_lower_right_bevel(tab_2d_indexes pawn_co, char c);
	bool check_lower_left_bevel(tab_2d_indexes pawn_co, char c);
	bool is_move_legal(tab_2d_indexes pawn_co, tab_2d_indexes move_co, char pawn_color, char field_color);
	bool is_capture(tab_2d_indexes pawn_co, tab_2d_indexes move_co, vector<tab_2d_indexes> captures);
	tab_2d_indexes get_captured_pawn(tab_2d_indexes pawn_co, tab_2d_indexes move_co);
	Player* get_current_player();
	Player* get_next_player();
	void add_pawn_to_take(tab_2d_indexes pawn);
	void find_pawns_to_take();
	void clear_pawns_to_take();
	bool draw();
	bool surrender();
	tab_2d_indexes take_pawn_to_move(char color_player_turn);
	tab_2d_indexes take_move(tab_2d_indexes pawn_co, vector<tab_2d_indexes> captures);
public:
	void missed_capture_mechanism();
	GameBrain(Player* player1, Player* player2, Board* board, ScoreBoard* score_board);
	virtual void make_move();
	void print_current_move_player();
	char get_current_color_move();
	char get_next_color_move();
	bool wanna_end();
	bool is_end(char c);
	void set_missed_capture(bool missed_capture);
	void end_game();
	int get_move_counter();
	bool get_computer_move();
	void set_computer_move(bool computer_move);
};

