#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void client_thread(int client_fd)
{
    char buffer[1024];
    int bytes;
    while ((bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0)
    {
        buffer[bytes] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }
    close(client_fd);
}

int main()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress = {};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);
    bind(server_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    listen(server_fd, 10);

    while (true)
    {
        int new_socket = accept(server_fd, NULL, NULL);
        if (new_socket >= 0)
        {
            std::thread(client_thread, new_socket).detach();
        }
    }

    close(server_fd);
    return 0;
}