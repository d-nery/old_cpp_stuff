#pragma once

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <string>

class UDPReceiver {
private:
	int socketfd;

	uint16_t port;

public:
	UDPReceiver(uint16_t port);
	void receive(void* data, int size);
	void receive(std::string &s);
};
