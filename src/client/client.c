#include "../shared/network/packet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char *argv[])
{
    const int fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    packet_t packet;
    packet.type = PACKET_TYPE_PONG;
    strncpy(packet.pong.y, "Hello, World!", 256);
    ssize_t bytes = sendto(fd, &packet, sizeof(packet), 0, (struct sockaddr *)&addr, sizeof(addr));

    close(fd);
    return 0;
}
