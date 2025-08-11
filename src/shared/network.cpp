#include "network.hpp"

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
    close(tcpSocket);
    close(udpSocket);
}

network::server::start()
{
    // Start listening on TCP socket
    if (listen(tcpSocket, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (true)
    {
    }
}

network::server::handleClient(int clientSocket)
{

}