#pragma once

#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include "packet.hpp"

namespace network
{
    class server
    {
    public:
        server(int port);
        ~server();
        void accept();
        void send(packet::packet &pkt, int clientSocket);
        void receive(int clientSocket);

    private:
        struct sockaddr_in address;
        int socket;
        std::vector<int> clientSockets;
    };

    class client
    {
    public:
        client(const std::string &host, int port);
        ~client();
        void send(packet::packet &pkt);
        void receive();

    private:
        struct sockaddr_in address;
        int socket;
    };
}