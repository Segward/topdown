#include "pch.h"
#include "util/debug.h"
#include "network/packet.h"

#define PORT 8080

int main(int argc, char *argv[])
{
    const int fd = socket(AF_INET, SOCK_DGRAM, 0);
    ASSERT_FALSE(fd, -1, "Failed to create socket");

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

    packet_t packet;
    packet.type = PACKET_TYPE_PING;
    strncpy(packet.ping.msg, "Hello, World!", MAX_STRING_SIZE);
    ssize_t bytes = sendto(fd, &packet, sizeof(packet),
       0, (struct sockaddr *)&addr, sizeof(addr));
    ASSERT_FALSE(bytes, -1, "Failed to send packet");

    close(fd);
    return 0;
}
