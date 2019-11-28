#include <iostream>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <mutex>

#include <ncurses.h>
#include <unistd.h>

#include "UDPSender.hh"
#include "UDPReceiver.hh"

using namespace std;

static WINDOW* input;
static WINDOW* output;

int x = 1;
mutex output_mtx;

void telaSaida(UDPReceiver& receiver) {
	string s;
	for (;;) {
		if (x >= LINES - LINES/10 - 6 - 1) {
			output_mtx.lock();
			x = 1;
			wclear(output);
			wmove(output, 1, 1);
			output_mtx.unlock();
		}
		receiver.receive(s);
		output_mtx.lock();
		wprintw(output, s.data());
		box(output, 0, 0);
		wmove(output, ++x, 1);
		wrefresh(output);
		output_mtx.unlock();
	}
}

void telaEntrada(UDPSender& sender) {
	string s;
	char j[1024];
	for (;;) {
		wgetstr(input, j);
		s = j;
		if (s.length() > 0) {
			if (x >= LINES - LINES/10 - 6 - 1) {
				output_mtx.lock();
				x = 1;
				wclear(output);
				wmove(output, 1, 1);
				output_mtx.unlock();
			}
			s += '\n';
			sender.send(s);
			output_mtx.lock();
			string sent = "\t\t\t" + s;
			wprintw(output, sent.data());
			box(output, 0, 0);
			wmove(output, ++x, 1);
			wrefresh(output);
			output_mtx.unlock();
		}
		usleep(10 * 1000);
		wclear(input);
		box(input, 0, 0);
		wmove(input, 1, 1);
		wrefresh(input);
	}
}

int main(int argc, char** argv) {
	string ip;
	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " <target ip>\n";
		cerr << "Setting target ip to 127.0.0.1\n";
		ip = "127.0.0.1";
		cout << "\nAperte Enter para continuar\n";
		cin.get();
	} else ip = argv[1];

	UDPSender sender(10000, ip.data());
	UDPReceiver receiver(10000);

	initscr();

	printw("Aperte Ctrl+C para sair!");
	refresh();

	input = newwin(LINES/10, COLS - 10, LINES - LINES/10, 5);
	box(input, 0 , 0);
	wmove(input, 1, 1);
	wrefresh(input);

	output = newwin(LINES - LINES/10 - 6, COLS - 10, 3, 5);
	box(output, 0, 0);
	wmove(output, 1, 1);
	wrefresh(output);

	thread saida(telaSaida, ref(receiver));
	thread entrada(telaEntrada, ref(sender));

	saida.join();
	entrada.join();

	endwin();
	return 0;
}
