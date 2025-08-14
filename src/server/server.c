#include "../shared/network/packet.h"
#include "../shared/util/assert.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

#define PORT 8080

int main(int argc, char *argv[])
{
  const int fd = socket(AF_INET, SOCK_DGRAM, 0);
  ASSERTF(fd, -1, "Failed to create socket");
  LOG("Socket created successfully");

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);
  int s = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
  ASSERT(s, 0, "Failed to bind socket");
  LOG("Socket bound successfully");

  while (1) {
    packet_t packet;
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    ssize_t bytes = recvfrom(fd, &packet, sizeof(packet), 0, (struct sockaddr *)&clientAddr, &addrLen);
    ASSERTF(bytes, -1, "Failed to receive packet");
    LOG("Received packet from %s:%d", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
  }

  close(fd);
  return 0;  
}
