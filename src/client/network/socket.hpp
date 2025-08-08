#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Socket
{
public:
    Socket();
    ~Socket();
    bool connect(const char *ip, int port);
    bool send(const char *message, size_t size);
    bool receive(char *buffer, size_t size);

private:
    int sockfd;
    struct sockaddr_in serverAddr;
};
