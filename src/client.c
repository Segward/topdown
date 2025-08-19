#include <SDL3/SDL.h>
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

void process_client_packets(client_t *client) {

}

void *client_loop(void *arg) {
  client_t *client = (client_t *)arg;
  packet_t packet;

  while (1) {
    ssize_t bytes = recv(client->fd, &packet, sizeof(packet), 0);
    if (bytes == 0)
      break;

    if (bytes < 0)
      continue;
    
    packet_channel_enqueue(&client->channel, &packet);
    printf("Received packet: %d bytes\n", (int)bytes);
  }

  close(client->fd);
  printf("Connection closed\n");
  return NULL;
}

int main(int argc, char *argv[]) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("socket");
    return 1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);

  if (inet_pton(AF_INET, SERVER_IP, 
      &server_addr.sin_addr) <= 0) {
    perror("inet_pton");
    close(fd);
    return 1;
  }

  if (connect(fd, (struct sockaddr *)&server_addr, 
      sizeof(server_addr)) < 0) {
    perror("connect");
    close(fd);
    return 1;
  }  

  client_t client = {0};
  client.fd = fd;
  client.channel.head = client.channel.tail = 0;

  world_t world = {0};
  world.player_count++;
  
  player_t player = {0};
  player.client_id = client.id = 0;

  world.players[0] = player;
  client.world = world;
  pthread_mutex_init(&client.channel.mtx, NULL); 

  pthread_t thread;
  if (pthread_create(&thread, NULL, 
      client_loop, &client) != 0) {
    perror("pthread_create");
    close(fd);
    return 1;
  }

  while (sleep(1) == 0) {

  }

  close(fd);
  return 0;
}
