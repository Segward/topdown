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
    void start(string ip, int port);


private:
    int sockfd;
    struct sockaddr_in serverAddr;
    void connect(int clientFd);
};
