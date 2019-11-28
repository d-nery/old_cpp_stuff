#pragma once

#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <string>

class UDPSender {
private:
	int socketfd;
	struct sockaddr_in remote;

	uint16_t port;
public:
	UDPSender(uint16_t port, const char* ip);
	void send(void* data, size_t length);
	void send(const std::string& str);
};
