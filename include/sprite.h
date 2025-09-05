#ifndef SPRITE_H
#define SPRITE_H

#include "pch.h"

#define MAX_PATH_SIZE 1024

typedef struct {
  const char path[MAX_PATH_SIZE];
  int width;
  int height;
} sprite_t;

#endif
