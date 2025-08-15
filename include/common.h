#pragma once

#include "pch.h"
#include "packet.h"

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

typedef struct {
  float x;
  float y;
} player_t;

typedef struct {
  float x;
  float y;
} entity_t;

#define MAX_ENTITIES 200

typedef struct {
  player_t players[MAX_CLIENTS];
  entity_t entities[MAX_ENTITIES];
  int player_count;
  int entity_count;
} world_t;
