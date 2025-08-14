#pragma once

#include "pch.h"
#include "util/debug.h"
#include "network/packet.h"

#define MAX_QUEUE_SIZE 1024

typedef struct {
  packet_t packets[MAX_QUEUE_SIZE];
  int head;
  int tail;
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int *clients;
} netqueue_t;

typedef struct {
  int fd;
  netqueue_t *queue;
} netqueue_args_t;

static inline void netqueue_init(netqueue_t *queue) {
  queue->head = 0;
  queue->tail = 0;
  pthread_mutex_init(&queue->mutex, NULL);
  pthread_cond_init(&queue->cond, NULL);
}

static inline int netqueue_enqueue(netqueue_t *queue, packet_t *packet) {
  pthread_mutex_lock(&queue->mutex);
  int next = (queue->tail + 1) % MAX_QUEUE_SIZE;  
  if (next == queue->head) {
    pthread_mutex_unlock(&queue->mutex);
    return -1;
  }

  queue->packets[queue->tail] = *packet;
  queue->tail = next;
  pthread_cond_signal(&queue->cond);
  pthread_mutex_unlock(&queue->mutex);
  return 0;
}

static inline int netqueue_dequeue(netqueue_t *queue, packet_t *packet) {
  pthread_mutex_lock(&queue->mutex);
  while (queue->head == queue->tail) {
    pthread_cond_wait(&queue->cond, &queue->mutex);
  }

  int next = (queue->head + 1) % MAX_QUEUE_SIZE;
  *packet = queue->packets[queue->head];
  queue->head = next;
  pthread_mutex_unlock(&queue->mutex);
  return 0;
}

static inline void *netqueue_thread(void *arg) {
  netqueue_args_t *args = (netqueue_args_t *)arg;
  netqueue_t *queue = args->queue;
  while (1) {
    packet_t packet;
    struct sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    ssize_t bytes = recvfrom(args->fd, &packet, sizeof(packet_t), 
      0, (struct sockaddr *)&clientAddr, &addrLen);
    if (bytes < 0) {
      ERROR("recvfrom failed: %s", strerror(errno));
      continue;
    }
    
    netqueue_enqueue(queue, &packet);
  }
}
