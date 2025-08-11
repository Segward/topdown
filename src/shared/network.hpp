#pragma once

#include "packet.hpp"
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <vector>

namespace network
{
    class server
    {
    public:
        server(int port);
        ~server();
        void send(packet::packet &pkt, struct sockaddr_in &client);
        packet::packet receive(struct sockaddr_in &client);

    private:
        struct sockaddr_in address;
        int socket;
        std::vector<struct sockaddr_in> clients;
    };

    class client
    {
    public:
        client(const std::string &host, int port);
        ~client();
        void send(packet::packet &pkt);
        packet::packet receive();

    private:
        struct sockaddr_in address;
        int socket;
    };
}