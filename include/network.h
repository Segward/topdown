#ifndef NETWORK_H
#define NETWORK_H

#include "pch.h"
#include "packet.h"
#include "world.h"
#include "channel.h"
#include "player.h"

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define MAX_CONNECTIONS 10

typedef struct {
  player_t *player;
  channel_t channel;
} connection_t;

typedef struct {
  connection_t connections[MAX_CONNECTIONS]; 
  int connection_count;
  channel_t channel;
} network_t;

int network_free_inactive(network_t *network);
int network_server_init(network_t *network);
int network_client_init(network_t *network);

#endif
