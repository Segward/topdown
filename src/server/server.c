#include "pch.h"
#include "debug.h"
#include "packet.h"

#define PORT 8080
#define MAX_CLIENTS 100

int receive_packet(int fd, packet_t *packet, 
  struct sockaddr_in *clientAddr)
{
  socklen_t addrLen = sizeof(*clientAddr);
  ssize_t bytes = recvfrom(fd, packet, sizeof(packet_t), 
    0, (struct sockaddr *)clientAddr, &addrLen);
  if (bytes < 0) {
    ERROR("Failed to receive packet: %s", strerror(errno));
    return -1;
  }

  return bytes;
}

int send_packet(int fd, const packet_t *packet, 
  const struct sockaddr_in *clientAddr)
{
  ssize_t bytes = sendto(fd, packet, sizeof(packet_t), 
    0, (const struct sockaddr *)clientAddr, sizeof(*clientAddr));
  if (bytes < 0) {
    ERROR("Failed to send packet: %s", strerror(errno));
    return -1;
  }

  return bytes;
}

int main(int argc, char *argv[])
{
  const int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    ERROR("Failed to create socket: %s", strerror(errno));
    return -1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);
  int s = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
  if (s < 0) {
    ERROR("Failed to bind socket: %s", strerror(errno));
    close(fd);
    return -1;
  }

  int clients[MAX_CLIENTS];
  int clientCount = 0;
  int playerId = 1;

  while (1) {
    packet_t packet;
    struct sockaddr_in clientAddr;
    ssize_t bytes = receive_packet(fd, &packet, &clientAddr);
    if (bytes < 0) {
      ERROR("Failed to receive packet: %s", strerror(errno));
      continue;
    }
    
    if (packet.type == PACKET_TYPE_CONNECT) {
      packet_t response;
      response.type = PACKET_TYPE_CONNECT;
      response.connect.playerId = playerId++;
      bytes = send_packet(fd, &response, &clientAddr);
      if (bytes < 0) {
        ERROR("Failed to send response packet: %s", strerror(errno));
        continue;
      }
    
      clients[clientCount++] = response.connect.playerId;
      LOG("Sent connect response with playerId %d", 
        response.connect.playerId);
    }

    for (int i = 0; i < clientCount; i++) {
      LOG("Client %d: %d", i, clients[i]);
    }

    LOG("Received packet from %s:%d", 
      inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port)); 
  }

  close(fd);
  return 0;  
}
