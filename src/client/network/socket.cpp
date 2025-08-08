#include "socket.hpp"
#include <iostream>
#include <unistd.h>
#include <sys/socket.h>

using namespace std;

Socket::Socket()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cerr << "Error creating socket" << endl;
    }

    serverAddr = {};
    serverAddr.sin_family = AF_INET;
}

Socket::~Socket()
{
    close(sockfd);
}

bool Socket::connect(const char *ip, int port)
{
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);
    int result = ::connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    return result == 0;
}

bool Socket::send(const char *message, size_t size)
{
    ssize_t bytesSent = ::send(sockfd, message, size, 0);
    return bytesSent == (ssize_t)size;
}

bool Socket::receive(char *buffer, size_t size)
{
    ssize_t bytesReceived = ::recv(sockfd, buffer, size, 0);
    return bytesReceived > 0;
}
