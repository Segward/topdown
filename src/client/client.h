#pragma once

#include "pch.h"
#include "player.h"

#define PORT 8080

typedef struct {
  const int serverFd;
  player_t player;
} client_t;
