#include <iostream>
#include <fstream>
#include <cstdlib>
#include <atomic>
#include <limits>
#include <thread>
#include <cctype>
#include <ctime>

#include <ncurses.h>

#include <SFML/Audio.hpp>

#include "utils.hpp"

using namespace std;

ifstream palavras("./Resources/palavras_forca.txt");
atomic<bool> ignchar(false);

void ign() {
	char c;
	for(;;)
		while(ignchar.load()) c = getch();
}

int getNumberLines() {
	char c;
	int i = 0;
	while (palavras.get(c))
	    if (c == '\n')
	        ++i;

	palavras.close();
	palavras.open("./Resources/palavras_forca.txt");
	return i;
}

string readLine(int linha) {
	string cont;
	for (int i = 0; getline(palavras, cont) && i < linha; ++i);
	palavras.close();
	palavras.open("./Resources/palavras_forca.txt");
	return cont;
}

int main(int argc, char** argv) {
	thread _ign(ign); _ign.detach();
	sf::Music errou, burro, naofacil;
	string palavra_atual, tentativas[4], asteriscos, mensagem;
	size_t achou;
	int tentativa;
	int vidas, linha, numLinhas = getNumberLines();
	size_t pos;
	bool flag;

	errou.openFromFile("./Resources/errou.wav");
	burro.openFromFile("./Resources/burro.wav");
	naofacil.openFromFile("./Resources/naofacil.wav");

	srand(time(NULL));

	initscr();
	printw("Aperte F2 para sair");
	int meioy = LINES/2, meiox = COLS/2;

	keypad(stdscr, TRUE);
	noecho();
	raw();
	for(;;) {
		linha = rand() % (numLinhas + 1);
		palavra_atual = readLine(linha);
		asteriscos = "";
		mensagem = "";
		tentativas[0] = "A B C D E F G";
		tentativas[1] = "H I J K L M N";
		tentativas[2] = "O P Q R S T U";
		tentativas[3] = "V W X Y Z";

		if (argc < 2) vidas = 6;
		else vidas = atoi(argv[1]);

		for (auto &i : palavra_atual) {
			if (i != ' ')
				asteriscos += '*';
			else
				asteriscos += ' ';
		}

		while (vidas > 0 && asteriscos.find('*') != string::npos) {
			clear();
			refresh();
			move(meioy - 2, meiox - asteriscos.length()/2);
			printw("%s", asteriscos.data());
			move(meioy, meiox - 5);
			printw("Vidas: %2d", vidas);
			move(meioy + 1, meiox - mensagem.length()/2);
			printw("%s", mensagem.data());

			refresh();
			while (naofacil.getStatus() == sf::SoundSource::Status::Playing) ignchar.store(true);
			ignchar.store(false);
			move(meioy + 2, meiox - 9);
			printw("Escolha uma letra:");
			for (int i = 0; i < 4; i++) {
				move(meioy + 4 + i, meiox - tentativas[i].length()/2);
				printw("%s", tentativas[i].data());
			}
			refresh();
			tentativa = getch();
			if (tentativa == KEY_F(2)) {endwin(); exit(1);}
			tentativa = toupper(tentativa);

			if (!(tentativa >= 'A' && tentativa <= 'Z')) {
				mensagem = "Insira uma letra válida";
				continue;
			}
			flag = false;
			for (int i = 0; i < 4; i++) {
				if ((pos = tentativas[i].find(tentativa)) != string::npos) {
					flag = true;
					tentativa = tolower(tentativa);
					tentativas[i][pos] = ' ';
					if ((achou = palavra_atual.find(tentativa)) == string::npos) {
						mensagem = "Letra incorreta!";
						vidas--;
						if (vidas > 1) errou.play();
						else if (vidas == 1) naofacil.play();
					} else {
						mensagem = "";
						while (achou != string::npos) {
							asteriscos[achou] = palavra_atual[achou];
							achou = palavra_atual.find(tentativa, achou + 1);
						}
					}
				}
			} if (!flag) mensagem = "Letra já jogada!";
		}
		clear();
		refresh();
		if (vidas <= 0) {
			burro.play();
			move(meioy - 1, meiox - 23/2);
			printw("Nossa, como você é burro!");
			move(meioy + 1, meiox - (15 + palavra_atual.length())/2);
			printw("A palavra era: %s", palavra_atual.data());
			refresh();
		} else {
			move(meioy - 1, meiox - 29/2);
			printw("Nossa, como você é inteligente!");
			move(meioy + 1, meiox - (15 + palavra_atual.length())/2);
			printw("A palavra era: %s", palavra_atual.data());
			refresh();
		}
		while (burro.getStatus() == sf::SoundSource::Status::Playing) ignchar.store(true);
		ignchar.store(false);
		move(meioy + 10, meiox - 29);
		printw("Aperte qualquer tecla para jogar novamente ou F2 para sair");
		refresh();
		if ((tentativa = getch()) == KEY_F(2)) break;
	}
	endwin();
}
