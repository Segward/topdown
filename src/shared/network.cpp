#include "network.hpp"
#include <iostream>

network::server::server(int tcpPort, int udpPort)
{
    // Create TCP and UDP sockets
    tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (tcpSocket < 0 || udpSocket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Allow address reuse (avoid "Address already in use")
    int opt = 1;
    setsockopt(tcpSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    setsockopt(udpSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // TCP socket binding
    tcpAddress.sin_family = AF_INET;
    tcpAddress.sin_addr.s_addr = INADDR_ANY;
    tcpAddress.sin_port = htons(tcpPort);
    if (bind(tcpSocket, (struct sockaddr *)&tcpAddress, sizeof(tcpAddress)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // UDP socket binding
    udpAddress.sin_family = AF_INET;
    udpAddress.sin_addr.s_addr = INADDR_ANY;
    udpAddress.sin_port = htons(udpPort);
    if (bind(udpSocket, (struct sockaddr *)&udpAddress, sizeof(udpAddress)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }
}

network::server::~server()
{
    // Close the TCP and UDP sockets
    close(tcpSocket);
    close(udpSocket);
}

void network::server::start()
{
    // Start listening on TCP socket
    if (listen(tcpSocket, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept a new client connection
    int clientSocket = accept(tcpSocket, nullptr, nullptr);
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
    tcpSocket = socket(AF_INET, SOCK_STREAM, 0);
    udpSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (tcpSocket < 0 || udpSocket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up TCP address
    tcpAddress.sin_family = AF_INET;
    inet_pton(AF_INET, host.c_str(), &tcpAddress.sin_addr);
    tcpAddress.sin_port = htons(port);

    // Set up UDP address
    udpAddress.sin_family = AF_INET;
    inet_pton(AF_INET, host.c_str(), &udpAddress.sin_addr);
    udpAddress.sin_port = htons(port);
}

network::client::~client()
{
    // Close the TCP and UDP sockets
    close(tcpSocket);
    close(udpSocket);
}

void network::client::connect()
{
    // Connect to the TCP server
    if (::connect(tcpSocket, (struct sockaddr *)&tcpAddress, sizeof(tcpAddress)) < 0)
    {
        perror("connect");
        exit(EXIT_FAILURE);
    }
}

void network::client::send(packet::packet &pkt)
{
    // Send header
    if (::send(tcpSocket, &pkt.header, sizeof(pkt.header), 0) < 0)
    {
        perror("send header");
        exit(EXIT_FAILURE);
    }

    // Send data
    if (::send(tcpSocket, pkt.data.data(), pkt.data.size(), 0) < 0)
    {
        perror("send data");
        exit(EXIT_FAILURE);
    }
}
