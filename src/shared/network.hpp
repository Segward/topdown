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
        void start();

    private:
        struct sockaddr_in address;
        int socket;
    };

    class client
    {
    public:
        client(const std::string &host, int port);
        ~client();
        void connect();
        void send(packet::packet &pkt);

    private:
        struct sockaddr_in address;
        int socket;
    };
}