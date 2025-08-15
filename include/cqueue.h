#pragma once

#include "pch.h"
#include "common.h"

void packet_channel_enqueue(channel_t *channel, packet_t *packet) {
  pthread_mutex_lock(&channel->mtx);
  int next_tail = (channel->tail + 1) % MAX_PACKETS;
  if (next_tail == channel->head) {
    fprintf(stderr, "Channel is full\n");
    pthread_mutex_unlock(&channel->mtx);
    return;
  }

  channel->packets[channel->tail] = *packet;
  channel->tail = next_tail;
  pthread_mutex_unlock(&channel->mtx);
}

void packet_channel_dequeue(channel_t *channel, packet_t *packet) {
  pthread_mutex_lock(&channel->mtx);
  if (channel->head == channel->tail) {
    fprintf(stderr, "Channel is empty\n");
    pthread_mutex_unlock(&channel->mtx);
    return;
  }

  *packet = channel->packets[channel->head];
  channel->head = (channel->head + 1) % MAX_PACKETS;
  pthread_mutex_unlock(&channel->mtx);
}
