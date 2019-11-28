#include <iostream>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <ctime>

#include <SFML/Audio.hpp>

#include "utils.hpp"

using namespace std;

ifstream palavras("./Resources/palavras_forca.txt");

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
	sf::Music errou, burro, naofacil;
	string palavra_atual, tentativas, asteriscos, mensagem;
	size_t achou;
	char tentativa;
	int vidas, linha, numLinhas = getNumberLines();

	errou.openFromFile("./Resources/errou.wav");
	burro.openFromFile("./Resources/burro.wav");
	naofacil.openFromFile("./Resources/naofacil.wav");

	srand(time(NULL));
	for(;;) {
		linha = rand() % (numLinhas + 1);
		palavra_atual = readLine(linha);
		asteriscos = "";
		mensagem = "";
		tentativas = "";

		if (argc < 2) vidas = 6;
		else vidas = atoi(argv[1]);

		cout << numLinhas << " " << linha << " " << palavra_atual << endl;
		for (auto &i : palavra_atual) {
			if (i != ' ')
				asteriscos += '*';
			else
				asteriscos += ' ';
		}

		while (vidas > 0 && asteriscos.find('*') != string::npos) {
			CLEAR;
			cout << '\t' << asteriscos << endl << endl;
			cout << "Vidas: " << vidas << endl;
			cout << "Tentativas: " << tentativas << endl << endl;
			cout << mensagem << endl;

			while (naofacil.getStatus() == sf::SoundSource::Status::Playing);
			cout << "Letras (minuscula): ";
			cin >> tentativa;

			if (!(tentativa >= 'a' && tentativa <= 'z')) {
				mensagem = "Insira uma letra válida!";
				continue;
			}

			if (tentativas.find(tentativa) == string::npos) {
				tentativas += tentativa;
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
			} else {
				mensagem = "Letra já jogada!";
			}
		}
		CLEAR;
		if (vidas <= 0) {
			burro.play();
			cout << "Suas vidas acabaram!!\nA palavra era: " << palavra_atual << endl << endl;
		} else {
			cout << "Parabéns! Você acertou!\n A palavra era: " << asteriscos << endl << endl;
		}
		while (burro.getStatus() == sf::SoundSource::Status::Playing);
		cout << "Aperte enter para jogar novamente. " << endl;
		cin.get();
		while(cin.get() != '\n');
	}
}
