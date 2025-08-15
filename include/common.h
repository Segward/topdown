#pragma once

#include "pch.h"
#include "packet.h"
#include "player.h"

#define MAX_PACKETS 100

typedef struct {
  packet_t packets[MAX_PACKETS];
  int head;
  int tail;
  pthread_mutex_t mtx;
} channel_t;

typedef struct {
  int fd;
  int id;
  player_t player;
  channel_t channel;
} client_t;

#define MAX_CLIENTS 100

typedef struct {
  int fd;
  client_t clients[MAX_CLIENTS];
  int count;
  pthread_mutex_t mtx;
} server_t;

#define PORT 8080
#define SERVER_IP "127.0.0.1"
