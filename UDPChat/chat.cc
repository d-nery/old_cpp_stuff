#include <iostream>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <mutex>

#include "UDPSender.hh"
#include "UDPReceiver.hh"

using namespace std;
mutex cout_mtx;

void sendr(UDPSender& sender) {
	string d;
	for (;;) {
		getline(cin, d);
		if (d.length() > 0)
			sender.send(d);
	}
}

void receiv(UDPReceiver& receiver) {
	string s;
	for (;;) {
		receiver.receive(s);
		cout << "\033[38;5;114m" << s << "\033[0m" << endl;
	}
}


int main(int argc, char** argv) {
	if (argc < 2) {
		cerr << "Usage: chat <target_ip>\n";
		exit(-1);
	}
	UDPSender sender(10000, argv[1]);
	UDPReceiver receiver(10000);

	system("clear");
	cout << "Bem-vindo ao chato louco \\o/\n\n";

	thread send(sendr, std::ref(sender));
	thread rec(receiv, std::ref(receiver));
	send.join();
	rec.join();
}
