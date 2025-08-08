#pragma once

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>

using namespace std;

class network
{
public:
    network();
    ~network();
    bool connect(string ip, int port);
    bool send(string message);
    bool receive(string &buffer);

private:
    int sockfd;
    struct sockaddr_in serverAddr;
};
