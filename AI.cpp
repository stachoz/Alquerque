#include "AI.h"
#include<time.h>

using namespace std;

AI::AI(Player* p1, Player* p2, Board* board, ScoreBoard* score_board) : GameBrain(p1, p2, board, score_board){}

void AI::make_move(){
	vector<tab_2d_indexes> moveable_pawns;
	vector<tab_2d_indexes> pawn_can_capture;
	char color = get_current_color_move();
	for (int i = 0; i < BOARD_SIZE; i+=2) {
		for (int j = 0; j < BOARD_SIZE; j += 2) {
			tab_2d_indexes pawn;
			pawn.x = i;
			pawn.y = j;
			if (board->get_board_element(i, j) == color) {
				if (are_there_moves(pawn, 'X')) moveable_pawns.push_back(pawn);
				vector<tab_2d_indexes> captures = find_all_captures(pawn);
				if (captures.size() > 0) pawn_can_capture.push_back(pawn);
			}
		}
	}
	int capture_size = pawn_can_capture.size();
	if (capture_size > 0) {
		int pawn_size = pawn_can_capture.size();
		tab_2d_indexes random_pawn = pawn_can_capture.at(random_number(capture_size));
		// wieloktorne bicie 
		do {
			vector<tab_2d_indexes> captures = find_all_captures(random_pawn);
			int captures_size = captures.size();
			tab_2d_indexes random_capture = captures.at(random_number(captures_size));
			capture(random_pawn, random_capture);
			coordinates c_pawn = convert(random_pawn);
			coordinates c_move = convert(random_capture);
			cout << "(" << c_pawn.letter << "," << c_pawn.number << ") "
				<< "-> (" << c_move.letter << "," << c_move.number << ")" << endl;
			random_pawn.x = random_capture.x;
			random_pawn.y = random_capture.y;
		} while (find_all_captures(random_pawn).size() > 0);
	} 
	else if(moveable_pawns.size() > 0){
		int random_pawn = random_number(moveable_pawns.size());
		tab_2d_indexes pawn = moveable_pawns.at(random_pawn);
		vector<tab_2d_indexes> moves = find_all_moves(pawn);
		int random_move = random_number(moves.size());
		tab_2d_indexes move_co = moves.at(random_move);
		move(pawn, move_co);
		coordinates c_pawn = convert(pawn);
		coordinates c_move = convert(move_co);
		cout << "(" << c_pawn.letter << "," << c_pawn.number << ") "
			<< "-> (" << c_move.letter << "," << c_move.number << ")" << endl;
	}
	move_counter++;
}

int AI::random_number(int max) {
	srand(time(NULL));
	return rand() % max;
}
