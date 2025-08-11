#include "network.hpp"
#include <iostream>
#include <thread>

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

    // Start listening on TCP socket
    if (listen(socket, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

network::server::~server()
{
    // Close the TCP socket
    close(socket);
}

void network::server::accept()
{
    // Accept a new client connection
    int clientSocket = ::accept(socket, nullptr, nullptr);
    if (clientSocket < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    std::thread t(&network::server::receive, this, clientSocket);
    t.detach();
}

void network::server::receive(int clientSocket)
{
    // Add the new client socket to the list
    clientSockets.push_back(clientSocket);
    while (true)
    {
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

        std::string str(pkt.data.begin(), pkt.data.end());
        std::cout << clientSocket << ": " << str << std::endl;
    }

    // Remove the client socket from the list
    clientSockets.erase(std::remove(clientSockets.begin(), clientSockets.end(), clientSocket), clientSockets.end());
    close(clientSocket);
}

void network::server::send(packet::packet &pkt, int clientSocket)
{
    // Send the packet header
    if (::send(clientSocket, &pkt.header, sizeof(pkt.header), 0) < 0)
    {
        perror("send header");
        return;
    }

    // Send the packet data
    if (::send(clientSocket, pkt.data.data(), pkt.data.size(), 0) < 0)
    {
        perror("send data");
        return;
    }
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

    // Connect to the TCP server
    if (::connect(socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
}

network::client::~client()
{
    // Close the TCP socket
    close(socket);
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

void network::client::receive()
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

    std::string str(pkt.data.begin(), pkt.data.end());
    std::cout << "Received response: " << str << std::endl;
}