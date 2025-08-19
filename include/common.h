#pragma once

#include "pch.h"
#include "packet.h"

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define MAX_PACKETS 100
#define MAX_CLIENTS 100

typedef struct {
  int client_id;
  float x;
  float y;
} player_t;

typedef struct {
  player_t players[MAX_CLIENTS];
  int player_count;
} world_t;

typedef struct {
  packet_t packets[MAX_PACKETS];
  int head;
  int tail;
  pthread_mutex_t mtx;
} channel_t;

typedef struct {
  int fd;
  int id;
  world_t world;
  channel_t channel;
} client_t;

typedef struct {
  client_t server_client;
  client_t clients[MAX_CLIENTS];
  int client_count;
  pthread_mutex_t mtx;
} server_t;
