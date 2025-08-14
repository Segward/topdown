#include "../shared/network/packet.h"
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
  if (fd < 0) {
    perror("socket");
    return 1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);
  if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("bind");
    close(fd);
    return 1;
  }

  packet_t packet;
  struct sockaddr_in clientAddr;
  socklen_t addrLen = sizeof(clientAddr);
  ssize_t bytes = recvfrom(fd, &packet, sizeof(packet), 0, (struct sockaddr *)&clientAddr, &addrLen);
  if (bytes < 0) {
    perror("recvfrom");
    close(fd);
    return 1;
  }

  printf("Received: %s\n", packet.pong.y);
  close(fd);
  return 0;  
}
