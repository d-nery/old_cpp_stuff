#include <iostream>
#include <atomic>
#include <thread>

#include "WordDatabase.hpp"
#include "Hangman.hpp"
#include "utils.hpp"

using namespace std;
using namespace forcalegal;

void config(WordDatabase& wdb) {
	int opcao = 1;
	string palavra;
	string _senha, senha = "senha123";
	cout << "Digite a senha para entrar no modo de configuração: ";
	cout << "\e[8m"; // Hidden
	cin >> _senha;
	CRESET;
	if (_senha == senha)
		for(;;) {
			CLEAR;
			cout << "Modo de configuração:\n"
				<< "\t1) Ver as palavras no banco de dados\n"
				<< "\t2) Adicionar palavra ao banco de dados\n"
				<< "\t3) Remover palavras do banco dados\n"
				<< "\t0) Iniciar jogo\n\n"
				<< "Escolha uma opcao: ";
			if (cin >> opcao) {
				switch (opcao) {
				case 0:
					return;

				case 1:
					cout << wdb << endl << "Aperte enter para continuar\n";
					cin.get(); cin.get();
					break;

				case 2:
					cout << "Digite uma palavra: ";
					cin >> palavra;
					switch (wdb.addWord(palavra)) {
						case 0: cout << "Palavra adicionada com sucesso!\n"; wdb.save(); break;
						case -1: cout << "Palavra já existente!\n"; break;
					}
					cout << "Aperte enter para continuar\n";
					cin.get(); cin.get();
					break;

				case 3:
					cout << "Digite uma palavra: ";
					cin >> palavra;
					switch (wdb.removeWord(palavra)) {
						case 0: cout << "Palavra removida com sucesso!\n"; wdb.save(); break;
						case -1: cout << "Palavra inexistente!\n"; break;
					}
					cout << "Aperte enter para continuar\n";
					cin.get(); cin.get();
					break;

				default:
					cin.clear();
					while (cin.get() != '\n');
				}
			} else {
				cin.clear();
				while (cin.get() != '\n');
			}
		}
	else
		cout << "Senha incorreta, saindo" << endl;
		exit(-1);
}

int main(int argc, char** argv) {
	srand(time(NULL));
	WordDatabase palavras("./Resources/Palavras.txt");

	if (argc == 2) {
		string opcao = argv[1];
		if (opcao.size() == 2 && opcao[0] == '-' && opcao[1] == 'd') {
			cout << "Entrando no modo de configuração..." << endl;
			config(palavras);
		} else {
			cerr << "Opção desconhecida " << opcao << "\nAperte enter para jogar" << endl;
			cin.get();
		}
	}

	Hangman forca(palavras);

	for (;;)
		forca.showScreen();

	return 0;
}
