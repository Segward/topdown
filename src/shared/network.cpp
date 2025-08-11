#include "network.hpp"

network::server::server(int port)
{
    // Create UDP socket
    socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up address structure
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind the socket
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
    // Send header
    if (::sendto(socket, &pkt.header, sizeof(pkt.header), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
    {
        perror("send header");
        exit(EXIT_FAILURE);
    }

    // Send data
    if (::sendto(socket, pkt.data.data(), pkt.data.size(), 0, (struct sockaddr *)&client, sizeof(client)) < 0)
    {
        perror("send data");
        exit(EXIT_FAILURE);
    }
}

packet::packet network::server::receive(struct sockaddr_in &client)
{
    packet::packet pkt;

    // Receive header from packet
    socklen_t clientLen = sizeof(client);
    ssize_t bytesRead = recvfrom(socket, &pkt.header, sizeof(pkt.header), 0, (struct sockaddr *)&client, &clientLen);
    if (bytesRead < static_cast<ssize_t>(sizeof(pkt.header)))
    {
        perror("recv header");
        exit(EXIT_FAILURE);
    }

    // Resize data vector from packet
    pkt.data.resize(pkt.header.size);
    bytesRead = recvfrom(socket, pkt.data.data(), pkt.header.size, 0, (struct sockaddr *)&client, &clientLen);
    if (bytesRead < static_cast<ssize_t>(pkt.header.size))
    {
        perror("recv data");
        exit(EXIT_FAILURE);
    }

    return pkt;
}

network::client::client(const std::string &host, int port)
{
    // Create UDP socket
    socket = ::socket(AF_INET, SOCK_DGRAM, 0);
    if (socket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up address structure
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
    // Send header
    if (::sendto(socket, &pkt.header, sizeof(pkt.header), 0, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("send header");
        exit(EXIT_FAILURE);
    }

    // Send data
    if (::sendto(socket, pkt.data.data(), pkt.data.size(), 0, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("send data");
        exit(EXIT_FAILURE);
    }
}

packet::packet network::client::receive()
{
    packet::packet pkt;

    // Receive header
    ssize_t bytesRead = recv(socket, &pkt.header, sizeof(pkt.header), 0);
    if (bytesRead < static_cast<ssize_t>(sizeof(pkt.header)))
    {
        perror("recv header");
        exit(EXIT_FAILURE);
    }

    // Resize data vector and receive data
    pkt.data.resize(pkt.header.size);
    bytesRead = recv(socket, pkt.data.data(), pkt.header.size, 0);
    if (bytesRead < static_cast<ssize_t>(pkt.header.size))
    {
        perror("recv data");
        exit(EXIT_FAILURE);
    }

    return pkt;
}