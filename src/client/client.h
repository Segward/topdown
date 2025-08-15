#pragma once

#include "pch.h"
#include "player.h"
#include "packet.h"

#define PORT 8080

typedef struct {
  const int serverFd;
  player_t player;
} client_t;

static inline int send_packet(client_t *client, 
  const struct sockaddr_in *addr, packet_t *packet) {
  ssize_t bytes = sendto(client->serverFd, packet, sizeof(*packet), 0, 
    (const struct sockaddr *)addr, sizeof(*addr));
  return bytes;
}

static inline int receive_packet(client_t *client,
  const struct sockaddr_in *addr, packet_t *packet) {
  socklen_t addrLen = sizeof(*addr);
  ssize_t bytes = recvfrom(client->serverFd, packet, sizeof(*packet), 0, 
    (struct sockaddr *)addr, &addrLen);
  return bytes;
}
