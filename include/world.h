#ifndef WORLD_H
#define WORLD_H

#include "pch.h"
#include "player.h"

#define MAX_PLAYERS 100

typedef struct {
  player_t players[MAX_PLAYERS];
  int player_count;
} world_t;

#endif

