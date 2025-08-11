#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

namespace network
{
    class server
    {
    public:
        server(int port);
        ~server();
        void start();
        void stop();
        void handleClient(int clientSocket);

    private:
        struct sockaddr_in tcpAddress;
        struct sockaddr_in udpAddress;
        int tcpSocket;
        int udpSocket;
    };

    class client
    {
    public:
        client(const std::string &host, int port);
        ~client();
        void connect();
        void disconnect();

    private:
        struct sockaddr_in tcpAddress;
        struct sockaddr_in udpAddress;
        int tcpSocket;
        int udpSocket;
    };
}