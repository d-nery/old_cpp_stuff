#include <iostream>
#include <cstdio>
#include <cstring>
#include <stdexcept>

#include "UDPReceiver.hh"

UDPReceiver::UDPReceiver(uint16_t port) {
	struct sockaddr_in local;

	this->port = port;

	if ((this->socketfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		std::cerr << "socket: " << strerror(errno) << std::endl;
		throw std::runtime_error("Socket: " + std::string(strerror(errno)));
	}

	memset((char *) &local, 0, sizeof(local));
	local.sin_family = AF_INET;	//IPv4
	local.sin_addr.s_addr = INADDR_ANY;
	local.sin_port = htons(this->port);

	if (bind(this->socketfd, (struct sockaddr *) &local, sizeof(local)) == -1) {
		std::cerr << "bind: " << strerror(errno) << std::endl;
		throw std::runtime_error("Bind: "  + std::string(strerror(errno)));
	}

	if (setsockopt(this->socketfd, SOL_SOCKET, SO_RCVBUF, NULL, sizeof(int)) == -1) {
		std::cerr << "setsockopt: " << strerror(errno) << std::endl;
		//throw std::runtime_error("Setsockopt: " + std::string(strerror(errno)));
	}
}

void UDPReceiver::receive(void* data, int size) {
	recv(this->socketfd, data, size, 0);
}

void UDPReceiver::receive(std::string &s) {
	size_t len;
	char *buf;

	recv(this->socketfd, &len, sizeof(len), 0);
	buf = new char[len];
	read(this->socketfd, buf, len);
	s.assign(buf, len);
	delete[] buf;
}
