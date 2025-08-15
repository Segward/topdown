#pragma once

#include "pch.h"
#include "player.h"
#include "packet.h"

#define MAX_PLAYERS 100
#define PORT 8080

typedef struct {
  const int fd;
  player_t players[MAX_PLAYERS];
  int playerCount;
} server_t;

static inline int player_exists(server_t *server, const struct sockaddr_in *addr) {
  for (int i = 0; i < server->playerCount; i++) {
    player_t *player = &server->players[i];
    if (player->addr.sin_addr.s_addr == addr->sin_addr.s_addr &&
        player->addr.sin_port == addr->sin_port) {
      return 0;
    }
  }
  return -1;
}

static inline int send_packet(server_t *server, 
  const struct sockaddr_in *addr, packet_t *packet) {
  ssize_t bytes = sendto(server->fd, packet, sizeof(packet_t), 0,
    (struct sockaddr *)addr, sizeof(*addr)); 
  return bytes;
}

static inline void server_add_player(server_t *server, const struct sockaddr_in *addr) {
  player_t *player = &server->players[server->playerCount++];
  player->addr = *addr;
  player->id = server->playerCount - 1;
  player->x = 0;
  player->y = 0;
}

static inline int receive_packet(server_t *server, packet_t *packet) {
  struct sockaddr_in addr;
  socklen_t addrSize = sizeof(addr);
  ssize_t bytes = recvfrom(server->fd, packet, sizeof(packet_t), 0,
    (struct sockaddr *)&addr, &addrSize);
  if (!player_exists(server, &addr))
    server_add_player(server, &addr);
  return bytes;
}

static inline void broadcast(server_t *server, packet_t *packet) {
  for (int i = 0; i < server->playerCount; i++) {
    player_t *player = &server->players[i];
    const struct sockaddr_in *addr = &player->addr;
    ssize_t bytes = send_packet(server, addr, packet);
  }
}
