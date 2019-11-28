#include <fstream>
#include <atomic>
#include <thread>

#include <ncurses.h>

#include "Hangman.hpp"

std::atomic<bool> ignchar(false);

void ign() {
	char c;
	for(;;)
		while(ignchar.load()) c = getch();
}

namespace forcalegal {
	Hangman::Hangman(WordDatabase& database, unsigned int chances) : database(database), chances(chances) {
		std::ifstream audiofile("../Resources/Audios.txt");
		std::string audioname;
		for (int i = 0; i < 3; i++) {
			getline(audiofile, audioname);
			audios[i].openFromFile(audioname);
		}

		word = database.getRandWord();
		_try = '-';
		initialChances = chances;

		maskWord();
		resetTries();
		initscr();
		raw();
		noecho();
		keypad(stdscr, TRUE);
		std::thread _ign(ign); _ign.detach();
	}

	Hangman::~Hangman() {
		endwin();
	}

	void Hangman::maskWord() {
		masked = "";
		for (auto &i : word) {
			if (i == ' ' || i == '-')
				masked += i;
			else
				masked += '*';
		}
	}

	void Hangman::resetTries() {
		tries[0] = "A B C D E F G";
		tries[1] = "H I J K L M N";
		tries[2] = "O P Q R S T U";
		tries[3] = "  V W X Y Z  ";
	}

	void Hangman::showScreen() {
		int meioy = LINES/2, meiox = COLS/2;
		while (chances > 0 && masked.find('*') != std::string::npos) {
			clear();
			refresh();
			mvprintw(meioy - 2, meiox - masked.length()/2, "%s", masked.data());
			mvprintw(meioy, meiox - 5, "chances: %2d", chances);
			mvprintw(meioy + 1, meiox - mesage.length()/2, "%s", mesage.data());
			refresh();

			while (audios[2].getStatus() == sf::SoundSource::Status::Playing) ignchar.store(true);
			ignchar.store(false);

			mvprintw(meioy + 2, meiox - 9, "Escolha uma letra:");
			for (int i = 0; i < 4; i++)
				mvprintw(meioy + 4 + i, meiox - tries[i].length()/2, "%s", tries[i].data());
			refresh();

			getTry();
		}
		clear();
		refresh();
		if(!won()) {
			audios[0].play();
			mvprintw(meioy - 1, meiox - 23/2, "Nossa, como você é burro!");
		} else {
			mvprintw(meioy - 1, meiox - 29/2, "Nossa, como você é inteligente!");
		}
		mvprintw(meioy + 1, meiox - (15 + word.length())/2, "A palavra era: %s", word.data());
		refresh();

		while (audios[0].getStatus() == sf::SoundSource::Status::Playing) ignchar.store(true);
		ignchar.store(false);

		mvprintw(meioy + 10, meiox - 29, "Aperte qualquer tecla para jogar novamente ou F2 para sair");
		refresh();
		if ((_try = getch()) == KEY_F(2)) {endwin(); exit(1);}

		word = database.getRandWord();
		chances = initialChances;
		maskWord();
		resetTries();
	}

	int Hangman::getTry() {
		_try = getch();
		if (_try == KEY_F(2)) {endwin(); exit(1);}
		_try = toupper(_try);

		if (!(_try >= 'A' && _try <= 'Z')) {
			mesage = "Insira uma letra válida";
			return -1;
		}
		bool flag = false;
		size_t pos, found;
		for (int i = 0; i < 4; i++) {
			if ((pos = tries[i].find(_try)) != std::string::npos) {
				flag = true;
				_try = tolower(_try);
				tries[i][pos] = ' ';
				if ((found = word.find(_try)) == std::string::npos) {
					mesage = "Letra incorreta!";
					chances--;
					if (chances > 1) audios[1].play();
					else if (chances == 1) audios[2].play();
				} else {
					mesage = "";
					while (found != std::string::npos) {
						masked[found] = word[found];
						found = word.find(_try, found + 1);
					}
				}
			}
		} if (!flag) mesage = "Letra já jogada!";
		return 0;
	}

	bool Hangman::won() {
		if (masked.find('*') == std::string::npos)
			return true;
		return false;
	}
}
