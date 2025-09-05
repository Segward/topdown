#ifndef CLIENT_H
#define CLIENT_H

#include "pch.h"
#include "network.h"
#include "world.h"
#include "player.h"

typedef struct {
  network_t network;
  world_t world;
  player_t *player;
} client_t;

#endif
