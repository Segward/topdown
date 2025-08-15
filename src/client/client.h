#pragma once

#include "pch.h"
#include "player.h"
#include "packet.h"

#define PORT 8080

typedef struct {
  int fd;
  struct sockaddr_in addr;
  player_t player;
} client_t;

static inline int send_packet(client_t *client, packet_t *packet) {
  struct sockaddr_in *addr = &client->addr;
  ssize_t bytes = sendto(client->fd, 
    packet, sizeof(*packet), 0, 
    (const struct sockaddr *)addr, sizeof(*addr));
  return bytes;
}

static inline int receive_packet(client_t *client, packet_t *packet) {
  struct sockaddr_in *addr = &client->addr;
  socklen_t addrLen = sizeof(*addr);
  ssize_t bytes = recvfrom(client->fd, 
    packet, sizeof(*packet), 0, 
    (struct sockaddr *)addr, &addrLen);
  return bytes;
}
