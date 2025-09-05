#ifndef CHANNEL_H
#define CHANNEL_H

#include "pch.h"
#include "packet.h"

#define CHANNEL_MAX_SIZE 1024

typedef struct {
  int head;
  int tail;
  packet_t packets[CHANNEL_MAX_SIZE];
  pthread_mutex_t mutex;
  pthread_t thread;
} channel_t;

int channel_enqueue(channel_t *channel, packet_t *packet);
int channel_dequeue(channel_t *channel, packet_t *packet);

#endif

