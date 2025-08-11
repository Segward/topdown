#include "network.hpp"
#include <iostream>

network::server::server(int port)
{
    // Create TCP and UDP sockets
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Allow address reuse (avoid "Address already in use")
    int opt = 1;
    setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // TCP socket binding
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

network::server::~server()
{
    // Close the TCP socket
    close(socket);
}

void network::server::start()
{
    // Start listening on TCP socket
    if (listen(socket, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept a new client connection
    int clientSocket = accept(socket, nullptr, nullptr);
    if (clientSocket < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Read the packet header
    packet::packet pkt;
    size_t bytesRead = recv(clientSocket, &pkt.header, sizeof(pkt.header), 0);
    if (bytesRead < sizeof(pkt.header))
    {
        perror("recv header");
        close(clientSocket);
        return;
    }

    // Read the packet data
    pkt.data.resize(pkt.header.size);
    bytesRead = recv(clientSocket, pkt.data.data(), pkt.header.size, 0);
    if (bytesRead < pkt.header.size)
    {
        perror("recv data");
        close(clientSocket);
        return;
    }

    // Output the packet data
    std::string str(pkt.data.begin(), pkt.data.end());
    std::cout << "Received packet data: " << str << std::endl;
    close(clientSocket);
}

network::client::client(const std::string &host, int port)
{
    // Create TCP and UDP sockets
    socket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (socket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up TCP address
    address.sin_family = AF_INET;
    inet_pton(AF_INET, host.c_str(), &address.sin_addr);
    address.sin_port = htons(port);
}

network::client::~client()
{
    // Close the TCP socket
    close(socket);
}

void network::client::connect()
{
    // Connect to the TCP server
    if (::connect(socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
}

void network::client::send(packet::packet &pkt)
{
    // Send header
    if (::send(socket, &pkt.header, sizeof(pkt.header), 0) < 0)
    {
        perror("send header");
        exit(EXIT_FAILURE);
    }

    // Send data
    if (::send(socket, pkt.data.data(), pkt.data.size(), 0) < 0)
    {
        perror("send data");
        exit(EXIT_FAILURE);
    }
}
