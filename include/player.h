#ifndef PLAYER_H
#define PLAYER_H

#include "pch.h"
#include "sprite.h"

typedef struct {
  int id;
  int x;
  int y;
  sprite_t sprite;
} player_t;

#endif
