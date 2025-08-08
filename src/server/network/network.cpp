#include "network.hpp"
#include <thread>
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

void network::start(string ip, int port)
{
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
    int result = bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (result < 0)
    {
        cerr << "Error binding socket" << endl;
        return;
    }

    result = listen(sockfd, SOMAXCONN);
    if (result < 0)
    {
        cerr << "Error listening on socket" << endl;
        return;
    }

    while (true)
    {
        int clientFd = accept(sockfd, nullptr, nullptr);
        if (clientFd < 0)
        {
            cerr << "Error accepting connection" << endl;
            continue;
        }
        thread(&network::connect, this, clientFd).detach();
    }
}

void network::connect(int clientFd)
{
    char buffer[1024];
    int bytes;
    while ((bytes = recv(clientFd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes] = '\0';
        cout << "Received: " << buffer << endl;
    }
    close(clientFd);
}
