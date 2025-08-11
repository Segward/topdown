#include "../shared/network.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    network::server srv(8080);

    while (true)
    {
        struct sockaddr_in client;
        packet::packet pkt = srv.receive(client);
        std::string message(pkt.data.begin(), pkt.data.end());
        std::cout << "Received message: " << message << std::endl;
        srv.broadcast(pkt);
    }

    return 0;
}