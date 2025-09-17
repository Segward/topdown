#include "pch.h"
#include "channel.h"

void *channel_loop(void *args) {
  channel_t *channel = (channel_t *)args;
  packet_t packet;

  while (1) {
    ssize_t bytes = recv(channel->fd, &packet, sizeof(packet), 0);
    if (bytes == 0)
      break;

    if (bytes < 0)
      continue;

    channel_enqueue(channel, &packet);
  }

  return NULL;
}

int channel_enqueue(channel_t *channel, packet_t *packet) {
  pthread_mutex_lock(&channel->mutex);
  int next_tail = (channel->tail + 1) % CHANNEL_MAX_SIZE;
  if (next_tail == channel->head) {
    pthread_mutex_unlock(&channel->mutex);
    return 0;
  }

  printf("packet enqueued\n");
  channel->packets[channel->tail] = *packet;
  channel->tail = next_tail;
  pthread_mutex_unlock(&channel->mutex);
  return 0;
}

int channel_dequeue(channel_t *channel, packet_t *packet) {
  pthread_mutex_lock(&channel->mutex);
  if (channel->head == channel->tail) {
    pthread_mutex_unlock(&channel->mutex);
    return 0;
  } 

  printf("packet dequeued\n");
  int next_head = (channel->head + 1) % CHANNEL_MAX_SIZE;
  *packet = channel->packets[channel->head];
  channel->head = next_head; 
  pthread_mutex_unlock(&channel->mutex);
  return 0;
}
