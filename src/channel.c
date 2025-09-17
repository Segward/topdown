#include "pch.h"
#include "channel.h"

void *channel_loop(void *args) {
  while (1) {
    sleep(1);
  }

  return NULL;
}

int channel_enqueue(channel_t *channel, packet_t *packet) {
  return 0;
}

int channel_dequeue(channel_t *channel, packet_t *packet) {
  return 0;
}
