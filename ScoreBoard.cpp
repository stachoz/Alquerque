#define _CRT_SECURE_NO_WARNINGS
#include "ScoreBoard.h"
#include "Player.h"
#include<iostream>
#include<fstream>
#include<ctime>

using namespace std;

void ScoreBoard::add_game_score(Player* p1, Player* p2, int white_score, int black_score) {
	file.open(file_name, ios::app);
	if (file.is_open()) {
		time_t currentTime = std::time(nullptr);
		tm* currentTimeInfo = std::localtime(&currentTime);
		char buffer[80];
		strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", currentTimeInfo);

		file << buffer << " " << p1->get_nick() << " vs " << p2->get_nick()
			<< " " << white_score << "-" << black_score;
		if (p1->get_has_surrendered()) file << " " << p1->get_nick() << " PODDAL";
		if (p2->get_has_surrendered()) file << " " << p2->get_nick() << " PODDAL";
		file << endl;
		file.close();
	}
	else {
		cout << "nie udalo sie otowrzyc pliku" << endl;
	}
}

void ScoreBoard::print_score_board() {
	cout << endl << "HISTORIA" << endl;
	read_file.open(file_name);;
	if (read_file.is_open()) {
		string line;
		while (getline(read_file, line)) {
			cout << line << endl;
		}
		cout << endl;
		read_file.close();
	}
	else {
		cout << "brak gier" << endl;
	}
	string line;
}

