#include "network.hpp"
#include <iostream>
#include <unistd.h>

using namespace std;

network::network()
{
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        cerr << "Error creating socket" << endl;
    }

    serverAddr = {};
    serverAddr.sin_family = AF_INET;
}

network::~network()
{
    close(sockfd);
}

bool network::connect(string ip, int port)
{
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
    int result = ::connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    return result == 0;
}

bool network::send(string message)
{
    ssize_t bytesSent = ::send(sockfd, message.c_str(), message.size(), 0);
    return bytesSent == (ssize_t)message.size();
}

bool network::receive(string &buffer)
{
    char tempBuffer[1024];
    ssize_t bytesReceived = ::recv(sockfd, tempBuffer, sizeof(tempBuffer), 0);
    if (bytesReceived > 0)
    {
        buffer.assign(tempBuffer, bytesReceived);
    }
    return bytesReceived > 0;
}
