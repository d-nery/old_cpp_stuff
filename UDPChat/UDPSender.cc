#include <iostream>
#include <cstdio>
#include <cstring>

#include "UDPSender.hh"

UDPSender::UDPSender(uint16_t port, const char* ip) {
	this->port = port;

	if ((this->socketfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		std::cerr << "socket: " << strerror(errno) << std::endl;
		// exit(-1);
	}

	memset((char *) &remote, 0, sizeof(remote));
	remote.sin_family = AF_INET;	//IPv4
	if (!inet_pton(AF_INET, ip, &remote.sin_addr)) {
		std::cerr << "inet_pton: " << strerror(errno) << std::endl;
		// exit(-1);
	}
	remote.sin_port = htons(this->port);

	if (connect(this->socketfd, (struct sockaddr *) &remote, sizeof(remote)) == -1) {
		std::cerr << "connect: " << strerror(errno) << std::endl;
		// exit(-1);
	}
}

void UDPSender::send(void* data, size_t length) {
	::send(this->socketfd, data, length, 0);
}

void UDPSender::send(const std::string& str) {
	size_t len = str.length();

	::send(this->socketfd, &len, sizeof(len), 0);
	::send(this->socketfd, str.data(), len, 0);
}
