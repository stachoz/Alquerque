#include "GameBrain.h"
#include<iostream>
#include<cctype>
#include<math.h>

using namespace std;

int GameBrain::move_counter = 0;
int GameBrain::white_pawns = 12;
int GameBrain::black_pawns = 12;
Board* GameBrain::board;
ScoreBoard* GameBrain::score_board;
bool GameBrain::computer_move = false;

GameBrain::GameBrain(Player* player1, Player* player2, Board* board, ScoreBoard* score_board){
	this->player1 = player1;
	this->player2 = player2;
	this->board = board;
	this->score_board = score_board;
}

void GameBrain::print_current_move_player() {
	if (get_current_color_move() == player1->get_letter_pawn_color()) {
		cout << "kolej gracza: " << player1->get_nick() << " (" << player1->get_letter_pawn_color() << ")" << endl;
	}
	if (get_current_color_move() == player2->get_letter_pawn_color()) {
		cout << "kolej gracza: " << player2->get_nick() << " (" << player2->get_letter_pawn_color() << ")" << endl;
	}
}

void GameBrain::make_move() {
	char foregin_color = get_next_color_move();
	char current_color = get_current_color_move();
	tab_2d_indexes pawn_co = take_pawn_to_move(current_color);
	vector<tab_2d_indexes> captures = find_all_captures(pawn_co);
	tab_2d_indexes move_co = take_move(pawn_co, captures);

	bool is_move_not_validated = true;
	// do while loop 
	if (is_capture(pawn_co, move_co, captures)) {
		capture(pawn_co, move_co);
		// move_co to nowe po³o¿enie pionka
		multi_capture_mechanism(move_co);
	}
	else {
		find_pawns_to_take();
		move(pawn_co, move_co);
	}
	this->move_counter++;
}

void GameBrain::capture(tab_2d_indexes pawn_co, tab_2d_indexes move_co) {
	char current_color = get_current_color_move();
	tab_2d_indexes captured_pawn = get_captured_pawn(pawn_co, move_co);
	board->set_board_element(captured_pawn.x, captured_pawn.y, 'X');
	board->set_board_element(move_co.x, move_co.y, current_color);
	board->set_board_element(pawn_co.x, pawn_co.y, 'X');
	if (current_color == 'B') black_pawns--;
	else if (current_color == 'C') white_pawns--;
}

void GameBrain::move(tab_2d_indexes pawn_co, tab_2d_indexes move_co) {
	char current_color = get_current_color_move();
	board->set_board_element(move_co.x, move_co.y, current_color);
	board->set_board_element(pawn_co.x, pawn_co.y, 'X');
}
	
tab_2d_indexes GameBrain::take_pawn_to_move(char color_player_turn) {
	bool is_not_validated = true;
	coordinates pawn_co;
	cout << "Wprowadz wspolrzedne pionka, ktorym chcesz sie ruszyc" << endl;
	while (is_not_validated) {
		pawn_co.letter = take_letter();
		pawn_co.number = take_number();
		tab_2d_indexes pawn_indexes = convert(pawn_co);
		vector<tab_2d_indexes> captures = find_all_captures(pawn_indexes);
		if (!validate_pawn(color_player_turn, pawn_indexes)) {
			cout << "bledne wspolrzedne " << endl; 
			continue;
		}
		if (are_there_moves(pawn_indexes, 'X') || captures.size() > 0) is_not_validated = false;
		else cout << "ten pionke nie moze sie ruszyc" << endl;
	}
	return convert(pawn_co);
}


tab_2d_indexes GameBrain::take_move(tab_2d_indexes pawn_co, vector<tab_2d_indexes> captures) {
	coordinates move;
	bool is_move_not_validated = true;
	char current_color = get_current_color_move();
	while (is_move_not_validated) {
		cout << "Podaj wspolrzedne pola na ktore chcesz przemiescic pionek" << endl;
		move.letter = take_letter();
		move.number = take_number();
		tab_2d_indexes move_indexes = convert(move);
		if (is_capture(pawn_co, move_indexes, captures)) is_move_not_validated = false;
		else if (is_move_legal(pawn_co, move_indexes, current_color, 'X')) {
			is_move_not_validated = false;
		}
		else cout << "zle wspolrzedne" << endl;
	}
	return convert(move);
}

bool GameBrain::validate_pawn(char color, tab_2d_indexes pawn_indexes) {
	if (board->get_board_element(pawn_indexes.x, pawn_indexes.y) == color) return true;
	return false;
}

bool GameBrain::validate_move(tab_2d_indexes pawn_co, tab_2d_indexes move_co) {
	int x_diff, y_diff;
	x_diff = abs(pawn_co.x - move_co.x);
	y_diff = abs(pawn_co.y - pawn_co.y);
	if (x_diff > 2 || y_diff > 2) return false;
	return true;
}

/*
	konwersja wspolrzednych na idneksy tablicy 2d
*/
tab_2d_indexes GameBrain::convert(coordinates co) {
	tab_2d_indexes indexes;
	indexes.x = co.number * 2;
	if (co.letter == 'a') indexes.y = 0;
	if (co.letter == 'b') indexes.y = 2;
	if (co.letter == 'c') indexes.y = 4;
	if (co.letter == 'd') indexes.y = 6;
	if (co.letter == 'e') indexes.y = 8;
	return indexes;
}

coordinates GameBrain::convert(tab_2d_indexes co) {
	coordinates c;
	if (co.y == 0) c.letter = 'a';
	if (co.y == 2) c.letter = 'b';
	if (co.y == 4) c.letter = 'c';
	if (co.y == 6) c.letter = 'd';
	if (co.y == 8) c.letter = 'e';
	c.number = co.x / 2;
	return c;
}

int GameBrain::take_number() {
	string input;
	cout << "liczba: ";
	getline(cin, input);
	//  konwersja char na int
	int number = input[0] - '0';
	while ((!cin) || input.length() > 1 || (!isdigit(input[0])) || number >= board->get_movable_fields() || number < 0) {
		cout << "zla liczba, wprowadz jeszcze raz: ";
		cin.clear();
		getline(cin, input);
		number = input[0] - '0';
	}
	return number;
}

char GameBrain::take_letter() {
	string input;
	cout << "litera: ";
	getline(cin, input);
	while ((!cin) || input.length() > 1 || input[0] < board->get_first_letter() || input[0] > board->get_last_letter()) {
		cout << "zla litera, wprowadz jeszcze raz: ";
		cin.clear();
		getline(cin, input);
	}
	return input[0];
}

char GameBrain::take_answer() {
	string input;
	char c = 'x';
	do {
		cout << "litera: ";
		getline(cin, input);
		if (input.length() == 1) {
			c = input[0];
			if (c != 't' && c != 'n') cout << "zla litera, wprowadz jeszcze raz" << endl;
		}
		else cout << "zla litera, wprawdz jeszcze raz" << endl;
	} while (c != 't' && c != 'n');
	return c;
}

bool GameBrain::are_there_moves(tab_2d_indexes pawn_co, char field_color){
	if (check_up(pawn_co, field_color) || check_down(pawn_co, field_color) || check_right(pawn_co, field_color) || check_left(pawn_co, field_color) ||
		check_lower_left_bevel(pawn_co, field_color) || check_lower_right_bevel(pawn_co, field_color) ||
		check_upper_left_bevel(pawn_co, field_color) || check_upper_right_bevel(pawn_co, field_color)) return true;
	return false;
}

char GameBrain::get_current_color_move() {
	if(this->move_counter % 2 == 0) {
		return player1->get_letter_pawn_color();
	}
	else {
		return player2->get_letter_pawn_color();
	}
}

char GameBrain::get_next_color_move() {
	if (player1->get_letter_pawn_color() == get_current_color_move()) return player2->get_letter_pawn_color();
	else return player1->get_letter_pawn_color();
}

bool GameBrain::check_up(tab_2d_indexes pawn_co, char c) {
	if (pawn_co.x > 0 && board->get_board_element(pawn_co.x - 2, pawn_co.y) == c) return true;
	return false;
}

bool GameBrain::check_down(tab_2d_indexes pawn_co, char c) {
	if (pawn_co.x < BOARD_SIZE - 1 && board->get_board_element(pawn_co.x + 2, pawn_co.y) == c) return true;
	return false;
}

bool GameBrain::check_left(tab_2d_indexes pawn_co, char c) {
	if (pawn_co.y != 0 && board->get_board_element(pawn_co.x, pawn_co.y - 2) == c) return true;
	return false;
}

bool GameBrain::check_right(tab_2d_indexes pawn_co, char c) {
	if (pawn_co.y != BOARD_SIZE - 1 && board->get_board_element(pawn_co.x, pawn_co.y + 2) == c) return true;
	return false;
}
bool GameBrain::check_upper_right_bevel(tab_2d_indexes pawn_co, char c) {
	if (pawn_co.x > 0 && pawn_co.y < BOARD_SIZE - 1 &&
		board->get_board_element(pawn_co.x - 1, pawn_co.y + 1) == '/' && 
		board->get_board_element(pawn_co.x - 2, pawn_co.y + 2) == c) return true;
	return false;
}

bool GameBrain::check_upper_left_bevel(tab_2d_indexes pawn_co, char c) {
	if(pawn_co.x > 0 && pawn_co.y > 0 && 
		board->get_board_element(pawn_co.x - 1, pawn_co.y - 1) == '\\' &&
		board->get_board_element(pawn_co.x - 2, pawn_co.y - 2) == c
		) return true;
	return false;
}

bool GameBrain::check_lower_right_bevel(tab_2d_indexes pawn_co, char c) {
	if (pawn_co.x < BOARD_SIZE - 1 && pawn_co.y < BOARD_SIZE - 1 &&
		board->get_board_element(pawn_co.x + 1, pawn_co.y + 1) == '\\' &&
		board->get_board_element(pawn_co.x + 2, pawn_co.y + 2) == c) return true;
	return false;
}

bool GameBrain::check_lower_left_bevel(tab_2d_indexes pawn_co, char c) {
	if (pawn_co.x < BOARD_SIZE - 1 && pawn_co.y > 0 &&
		board->get_board_element(pawn_co.x + 1, pawn_co.y - 1) == '/' && 
		board->get_board_element(pawn_co.x + 2, pawn_co.y - 2) == c) return true;
	return false;
}

bool GameBrain::is_move_legal(tab_2d_indexes pawn_co, tab_2d_indexes move_co, char pawn_color, char field_color) {
	int x_diff, y_diff;
	x_diff = move_co.x - pawn_co.x;
	y_diff = move_co.y - pawn_co.y;
	if (x_diff == -2 && y_diff == 0 && check_up(pawn_co, field_color)) return true;
	if (x_diff == 2 && y_diff == 0 && check_down(pawn_co, field_color)) return true;
	if (x_diff == 0 && y_diff == -2 && check_left(pawn_co, field_color)) return true;
	if (x_diff == 0 && y_diff == 2 && check_right(pawn_co, field_color)) return true;
	if (x_diff == -2 && y_diff == -2 && check_upper_left_bevel(pawn_co, field_color)) return true;
	if (x_diff == -2 && y_diff == 2 && check_upper_right_bevel(pawn_co, field_color)) return true;
	if (x_diff == 2 && y_diff == -2 && check_lower_left_bevel(pawn_co, field_color)) return true;
	if (x_diff == 2 && y_diff == 2 && check_lower_right_bevel(pawn_co, field_color)) return true;
	return false;
}
// zwraca pola na ktorych znajdzie sie pionek po zbiciu 
vector<tab_2d_indexes> GameBrain::find_all_captures(tab_2d_indexes pawn_co) {
	vector<tab_2d_indexes> captures;
	char next_color = get_next_color_move();
	tab_2d_indexes up, left, down, right, upper_left_bevel, upper_right_bevel, lower_left_bevel, lower_right_bevel;
	up.x = pawn_co.x - 2;
	up.y = pawn_co.y;
	down.x = pawn_co.x + 2;
	down.y = pawn_co.y;
	right.x = pawn_co.x;
	right.y = pawn_co.y + 2;
	left.x = pawn_co.x;
	left.y = pawn_co.y - 2;
	upper_left_bevel.x = pawn_co.x - 2;
	upper_left_bevel.y = pawn_co.y - 2;
	upper_right_bevel.x = pawn_co.x - 2;
	upper_right_bevel.y = pawn_co.y + 2;
	lower_left_bevel.x = pawn_co.x + 2;
	lower_left_bevel.y = pawn_co.y - 2;
	lower_right_bevel.x = pawn_co.x + 2;
	lower_right_bevel.y = pawn_co.y + 2;
	// czy w danym kierunku jest przecinwy pionek -> czy tym samym kierunku za przeciwnym pionkiem jest wolne pole
	if (check_up(pawn_co, next_color) && check_up(up, 'X')) {
		up.x -= 2;
		captures.push_back(up);
	}
	if (check_down(pawn_co, next_color) && check_down(down, 'X')){
		down.x += 2;
		captures.push_back(down);
	}
	if (check_left(pawn_co, next_color) && check_left(left, 'X')) {
		left.y -= 2;
		captures.push_back(left);
	}
	if (check_right(pawn_co, next_color) && check_right(right, 'X')) {
		right.y += 2;
		captures.push_back(right);
	}
	if (check_upper_right_bevel(pawn_co, next_color) && check_upper_right_bevel(upper_right_bevel, 'X')) {
		upper_right_bevel.x -= 2;
		upper_right_bevel.y += 2;
		captures.push_back(upper_right_bevel);
	}
	if (check_upper_left_bevel(pawn_co, next_color) && check_upper_left_bevel(upper_left_bevel, 'X')) {
		upper_left_bevel.x -= 2;
		upper_left_bevel.y -= 2;
		captures.push_back(upper_left_bevel);
	}
	if (check_lower_right_bevel(pawn_co, next_color) && check_lower_right_bevel(lower_right_bevel, 'X')) {
		lower_right_bevel.x += 2;
		lower_right_bevel.y += 2;
		captures.push_back(lower_right_bevel);
	}
	if (check_lower_left_bevel(pawn_co, next_color) && check_lower_left_bevel(lower_left_bevel, 'X')) {
		lower_left_bevel.x += 2;
		lower_left_bevel.y -= 2;
		captures.push_back(lower_left_bevel);
	}
	return captures;
}

bool GameBrain::is_capture(tab_2d_indexes pawn_co, tab_2d_indexes move_co, vector<tab_2d_indexes> captures) {
	// tab_2d_indexes captured_pawn = get_captured_pawn(pawn_co, move_co);
	for (tab_2d_indexes capture : captures) {
		if (move_co.x == capture.x && move_co.y == capture.y) return true;
	}
	return false;
}

tab_2d_indexes GameBrain::get_captured_pawn(tab_2d_indexes pawn_co, tab_2d_indexes move_co) {
	tab_2d_indexes captured_pawn;
	int x_diff, y_diff;
	x_diff = (move_co.x - pawn_co.x) / 2;
	y_diff = (move_co.y - pawn_co.y) / 2;
	captured_pawn.x = move_co.x - x_diff;
	captured_pawn.y = move_co.y - y_diff;
	return captured_pawn;
}

void GameBrain::multi_capture_mechanism(tab_2d_indexes pawn_co) {
	while (true) {
		vector<tab_2d_indexes> captures = find_all_captures(pawn_co);
		if (captures.size() == 0) break;
		cout << "kolejne bicie, wpisz 't' aby wykonac" << endl;
		char answer = take_answer();
		board->print();
		if (answer == 't') {
			bool is_not_capture = true;
			while (is_not_capture) {
				tab_2d_indexes move_indexes = take_move(pawn_co, captures);
				if (is_capture(pawn_co, move_indexes, captures)) {
					capture(pawn_co, move_indexes);
					// koordynaty ruchu to nowe wspolrzedne pionka
					pawn_co.x = move_indexes.x;
					pawn_co.y = move_indexes.y;
					is_not_capture = false;
				}
				else cout << "ten ruch nie jest biciem" << endl;
			}
		}
		else break;
		board->print();
	}
}

void GameBrain::find_pawns_to_take() {
	// player did not capture
	char current_color = get_current_color_move();
	for (int i = 0; i < BOARD_SIZE; i+=2) {
		for(int j = 0; j < BOARD_SIZE; j+=2) {
			char board_element = board->get_board_element(i, j);
			if (board_element == current_color) {
				tab_2d_indexes pawn;
				pawn.x = i;
				pawn.y = j;
				vector<tab_2d_indexes> captures = find_all_captures(pawn);
				if (captures.size() != 0) add_pawn_to_take(pawn);
			}
		}
	}
}

void GameBrain::set_missed_capture(bool missed_capture) {
	this->missed_capture = missed_capture;
}

void GameBrain::add_pawn_to_take(tab_2d_indexes pawn) {
	this->pawns_to_take.push_back(pawn);
}

void GameBrain::clear_pawns_to_take() {
	this->pawns_to_take.clear();
}

void GameBrain::missed_capture_mechanism() {
	//char enemy_color = get_next_color_move();
	string next_player = get_next_player()->get_nick();
	if (pawns_to_take.size() > 0) {
		cout << next_player << " gracz ominal bicie, zdjac jego pionek? (t) aby potwierdzic" << endl;
		char answer = take_answer();
		if (answer == 't') {
			bool is_pawn_not_good = true;
			while (is_pawn_not_good) {
				cout << "ktory pionek checsz zdjac?" << endl;
				coordinates pawn;
				pawn.letter = take_letter();
				pawn.number = take_number();
				tab_2d_indexes pawn_to_take_off = convert(pawn);
				for (tab_2d_indexes p : pawns_to_take) {
					if (p.x == pawn_to_take_off.x && p.y == pawn_to_take_off.y) {
						is_pawn_not_good = false;
						board->set_board_element(p.x, p.y, 'X');
						board->print();
						break;
					}
				}
				if (is_pawn_not_good) {
					cout << "ten pionek nie moze byc zdjety" << endl;
				}
			}
		}
	}
	clear_pawns_to_take();
}

bool GameBrain::draw() {
	this->print_current_move_player();
	char current_player_color = get_current_color_move();
	cout << get_current_player()->get_nick() << " chce remis" << endl;
	cout << get_next_player()->get_nick() << " wpisz 't' aby sie zgodzic, 'n' by odrzucic" << endl;
	char answer;
	answer = take_answer();
	if (answer == 't') {
		cout << "REMIS" << endl;
		return true;
	}
	return false;
}

bool GameBrain::surrender() {
	cout << "na pewno chcesz sie poddac? wpisz 't' aby potwierdzic, 'n' aby odrzucic" << endl;
	char answer;
	answer = take_answer();
	Player* current_player = get_current_player();
	Player* next_player = get_next_player();
	if (answer == 't') {
		cout << current_player->get_nick() << " poddal gre" << endl;
		cout << next_player->get_nick() << " wygral!" << endl;
		current_player->set_has_surrendered(true);
		return true;
	}
	return false;
}

Player* GameBrain::get_current_player() {
	char current_player_color = get_current_color_move();
	if (player1->get_letter_pawn_color() == current_player_color) return player1;
	else return player2;
}

Player* GameBrain::get_next_player() {
	char current_player_color = get_current_color_move();
	if (player1->get_letter_pawn_color() == current_player_color) return player2;
	else return player1;
}

bool GameBrain::can_player_move(char c){
	for (int i = 0; i < BOARD_SIZE; i+=2) {
		for(int j = 0; j < BOARD_SIZE; j+=2) {
			tab_2d_indexes pawn_co;
			pawn_co.x = i;
			pawn_co.y = j;
			if (board->get_board_element(i, j) == c && are_there_moves(pawn_co, 'X')) return true;
		}
	}
	return false;
}

bool GameBrain::can_capture(char c) {
	for (int i = 0; i < BOARD_SIZE; i += 2) {
		for (int j = 0; j < BOARD_SIZE; j += 2) {
			if (board->get_board_element(i, j) == c) {
				tab_2d_indexes pawn;
				pawn.x = i;
				pawn.y = j;
				vector<tab_2d_indexes> captures = find_all_moves(pawn);
				if (captures.size() > 0) return true;
			}
		}
	}
	return false;
}

bool GameBrain::is_end(char c) {
	if (white_pawns == 0 || black_pawns == 0) {
		return true;
	}
	return !(can_capture(c) || can_player_move(c));
}

bool GameBrain::wanna_end() {
	string input;
	int number;
	while(true){
		cout << "wprowadz odpowiednia liczbe" << endl;
		cout << "0 - kontynuuj" << endl;
		cout << "1 - remis" << endl;
		cout << "2 - poddanie" << endl;
		cout << "3 - komputer wykona ruch" << endl;
		getline(cin, input);
		// cast char number into int
		number = input[0] - '0';
		while ((!cin) || input.length() > 1 || (!isdigit(input[0])) || number > 3 || number < 0) {
			cout << "zla liczba, podaj jeszcze raz: ";
			cin.clear();
			getline(cin, input);
			number = input[0] - '0';
		}
		if (number == 0) return false;
		else if (number  == 1) {
			if (draw()) return true;
		}
		else if (number == 2) {
			if (surrender()) return true;
		} 
		else if (number == 3) {
			computer_move = true;
			return false;
		}
	}
	return false;
}

void GameBrain::end_game() {
	int white_score = basic_pawns_num - black_pawns;
	int black_score = basic_pawns_num - white_pawns;
	score_board->add_game_score(player1, player2, white_score, black_score);
}

vector<tab_2d_indexes> GameBrain::find_all_moves(tab_2d_indexes pawn_co) {
	vector<tab_2d_indexes> moves;
	tab_2d_indexes up, left, down, right, upper_left_bevel, upper_right_bevel, lower_left_bevel, lower_right_bevel;
	up.x = pawn_co.x - 2;
	up.y = pawn_co.y;
	down.x = pawn_co.x + 2;
	down.y = pawn_co.y;
	right.x = pawn_co.x;
	right.y = pawn_co.y + 2;
	left.x = pawn_co.x;
	left.y = pawn_co.y - 2;
	upper_left_bevel.x = pawn_co.x - 2;
	upper_left_bevel.y = pawn_co.y - 2;
	upper_right_bevel.x = pawn_co.x - 2;
	upper_right_bevel.y = pawn_co.y + 2;
	lower_left_bevel.x = pawn_co.x + 2;
	lower_left_bevel.y = pawn_co.y - 2;
	lower_right_bevel.x = pawn_co.x + 2;
	lower_right_bevel.y = pawn_co.y + 2;
	// check if foreign color is in near area and then check if this pawn can move 
	if (check_up(pawn_co, 'X')) moves.push_back(up); // 
	if (check_down(pawn_co, 'X')) moves.push_back(down);
	if (check_left(pawn_co, 'X')) moves.push_back(left);
	if (check_right(pawn_co, 'X')) moves.push_back(right);
	if (check_upper_right_bevel(pawn_co, 'X')) moves.push_back(upper_right_bevel); 
	if (check_upper_left_bevel(pawn_co, 'X')) moves.push_back(upper_left_bevel);
	if (check_lower_right_bevel(pawn_co, 'X')) moves.push_back(lower_right_bevel);
	if (check_lower_left_bevel(pawn_co, 'X')) moves.push_back(lower_left_bevel);
	return moves;
}

int GameBrain::get_move_counter() { return move_counter; }
bool GameBrain::get_computer_move() { return computer_move; }
void GameBrain::set_computer_move(bool computer_move) { this->computer_move = computer_move; }
