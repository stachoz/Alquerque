#include "Game.h"
#include "GameBrain.h"
#include "AI.h"
#include<iostream>
#include<string>
#include<vector>
#include<cctype>
using namespace std;

void Game::game() {
	

	while (true) {
		ScoreBoard* score_board = new ScoreBoard();
		cout << "1 - nowa gra" << endl;
		cout << "2 - historia gier" << endl;
		cout << "3 - wyjdz" << endl;
		int choice = get_menu_choice();
		if (choice == 1) {
			Board* board = new Board();
			this->set_board(board);
			/*
			string nick1 = "p1";
			string nick2 = "p2";
			*/
			cout << "gracz numer 1 - biale pionki" << endl;
			string nick1 = get_user_nick("");
			cout << "gracz numer 2 - czarne pionki" << endl;
			string nick2 = get_user_nick("");
			while (nick1.compare(nick2) == 0) {
				nick2 = get_user_nick("nazwy graczy musza sie roznic");
			}
			Player* player1 = new Player(nick1, 'B');
			Player* player2 = new Player(nick2, 'C');
			GameBrain* game_brain = new GameBrain(player1, player2, board, score_board);
			AI* ai = new AI(player1, player2, board, score_board);
			greet_players(player1, player2);
			board->print();
			while (true) {
				char next_player_turn_color = game_brain->get_next_color_move();
				char current_color = game_brain->get_current_color_move();
				game_brain->print_current_move_player();
				// wanna_end() to tak¿e 'menu kolejki'
				if (game_brain->wanna_end()) {
					// end game
					break;
				}
				game_brain->missed_capture_mechanism();
				if (game_brain->get_computer_move()) {
					ai->make_move();
					game_brain->set_computer_move(false);
				}
				else game_brain->make_move();
				board->print();
				// sprawdzanie na podstawie current_color bo po wykonaniu funkcji make_move()
				// licznik ruchów jest zwiêkszony
				if (game_brain->is_end(current_color)) {
					cout << "KONIEC GRY!" << endl;
					break;
				}
			}
			game_brain->end_game();
		}
		else if (choice == 2) {
			score_board->print_score_board();
		}
		else if (choice == 3) break;
	}
}

string Game::get_user_nick(string msg) {
	string nick;
	cout << msg << endl;
	do {
		cout << "nick: ";
		getline(cin, nick);
	} while (!validate_nick(nick));
	return nick;
}

int Game::get_menu_choice() {
	string input;
	cout << "liczba: ";
	getline(cin, input);
	int number = input[0] - '0';
	while ((!cin) || input.length() > 1 || (!isdigit(input[0])) || number > 3 || number < 1) {
		cout << "zla liczba, podaj jeszcze raz: ";
		cin.clear();
		//cin >> input;
		getline(cin, input);
		number = input[0] - '0';
	}
	return number;
}

bool Game::validate_nick(string nick) {
	bool is_alnum = false;
	// nick.lenght() < 2 to is_alnum = false
	for (int i = 0; i < nick.length() && nick.length() > 2; i++) {
		if (isalnum(nick.at(i))) {
			is_alnum = true;
			break;
		}
	}
	return is_alnum;
}

void Game::greet_players(Player* p1, Player* p2) {
	cout << "Alquerque " << p1->get_nick() << "(" << p1->get_full_pawn_color() << ")"
		<< " vs " << p2->get_nick() << " (" << p2->get_full_pawn_color() << ")" << endl;;
}

void Game::set_board(Board* board) {
	this->board = board;
}

void Game::set_game_brain(GameBrain* game_brain) {
	this->game_brain = game_brain;
}
