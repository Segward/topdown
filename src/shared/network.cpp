#include "network.hpp"

network::server::server(int port)
{
    socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (::bind(socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

network::server::~server()
{
    close(socket);
}

void network::server::send(packet::packet &pkt, struct sockaddr_in &client)
{
    if (::sendto(socket, &pkt.header, sizeof(pkt.header), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
    {
        perror("send header");
        exit(EXIT_FAILURE);
    }

    if (::sendto(socket, pkt.data.data(), pkt.data.size(), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
    {
        perror("send data");
        exit(EXIT_FAILURE);
    }
}

packet::packet network::server::receive(struct sockaddr_in &client)
{
    packet::packet pkt;
    socklen_t clientLen = sizeof(client);
    ssize_t bytesRead = recvfrom(socket, &pkt.header, sizeof(pkt.header), 0, (struct sockaddr *)&client, &clientLen);
    if (bytesRead < static_cast<ssize_t>(sizeof(pkt.header)))
    {
        perror("recv header");
        exit(EXIT_FAILURE);
    }

    pkt.data.resize(pkt.header.size);
    bytesRead = recvfrom(socket, pkt.data.data(), pkt.header.size, 0, (struct sockaddr *)&client, &clientLen);
    if (bytesRead < static_cast<ssize_t>(pkt.header.size))
    {
        perror("recv data");
        exit(EXIT_FAILURE);
    }

    bool clientKnown = false;
    for (struct sockaddr_in knownClient : clients)
    {
        if (knownClient.sin_addr.s_addr == client.sin_addr.s_addr &&
            knownClient.sin_port == client.sin_port)
        {
            clientKnown = true;
            break;
        }
    }

    if (!clientKnown)
    {
        clients.push_back(client);
    }

    return pkt;
}

void network::server::broadcast(packet::packet &pkt)
{
    for (struct sockaddr_in &client : clients)
    {
        send(pkt, client);
    }
}

std::vector<struct sockaddr_in> network::server::getClients()
{
    return clients;
}

void network::server::disconnect(struct sockaddr_in &client)
{
    int index = -1;
    for (size_t i = 0; i < clients.size(); ++i)
    {
        if (clients[i].sin_addr.s_addr == client.sin_addr.s_addr &&
            clients[i].sin_port == client.sin_port)
        {
            index = static_cast<int>(i);
            break;
        }
    }

    if (index != -1)
    {
        clients.erase(clients.begin() + index);
    }
}

network::client::client(const std::string &host, int port)
{
    socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    inet_pton(AF_INET, host.c_str(), &address.sin_addr);
    address.sin_port = htons(port);
}

network::client::~client()
{
    close(socket);
}

void network::client::send(packet::packet &pkt)
{
    if (::sendto(socket, &pkt.header, sizeof(pkt.header), 0, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("send header");
        exit(EXIT_FAILURE);
    }

    if (::sendto(socket, pkt.data.data(), pkt.data.size(), 0, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("send data");
        exit(EXIT_FAILURE);
    }
}

packet::packet network::client::receive()
{
    packet::packet pkt;
    ssize_t bytesRead = recv(socket, &pkt.header, sizeof(pkt.header), 0);
    if (bytesRead < static_cast<ssize_t>(sizeof(pkt.header)))
    {
        perror("recv header");
        exit(EXIT_FAILURE);
    }

    pkt.data.resize(pkt.header.size);
    bytesRead = recv(socket, pkt.data.data(), pkt.header.size, 0);
    if (bytesRead < static_cast<ssize_t>(pkt.header.size))
    {
        perror("recv data");
        exit(EXIT_FAILURE);
    }

    return pkt;
}