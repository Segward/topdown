#include <sqlite3.h>
#include "pch.h"
#include "common.h"
#include "cqueue.h"

void process_client_packets(client_t *client) {
  packet_t packet;
  while (client->channel.head != client->channel.tail) {
    packet_channel_dequeue(&client->channel, &packet);
    printf("Processing packet from client %d: type=%d\n", 
           client->id, packet.type);
  }
}

void *server_client_loop(void *args) {
  client_t *client = (client_t *)args;
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
  printf("Client %d disconnected\n", client->id);
  return NULL;
}

void *server_loop(void *args) {
  server_t *server = (server_t *)args;
  int fd = server->fd;
  struct sockaddr_in client_addr;
  socklen_t addr_len = sizeof(client_addr);
  
  while (1) {
    int client_fd = accept(fd, 
      (struct sockaddr *)&client_addr, &addr_len);
    if (client_fd < 0) {
      perror("accept");
      continue;
    }

    pthread_mutex_lock(&server->mtx);
    if (server->count >= MAX_CLIENTS) {
      pthread_mutex_unlock(&server->mtx);
      close(client_fd);
      continue;
    }

    int id = server->count++;
    client_t *client = &server->clients[id];
    client->fd = client_fd;
    client->id = id;
    client->channel.head = client->channel.tail = 0;
    pthread_mutex_init(&client->channel.mtx, NULL);

    pthread_t thread;
    if (pthread_create(&thread, NULL, 
        server_client_loop, client) != 0) {
      perror("pthread_create");
      close(client_fd);
      pthread_mutex_unlock(&server->mtx);
      continue;
    }

    pthread_detach(thread);
    pthread_mutex_unlock(&server->mtx);
    printf("Client %d connected\n", id);
  }
  
  return 0;
}

int main(int argc, char *argv[]) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("socket");
    return 1;
  }

  int opt = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, 
      &opt, sizeof(opt)) < 0) {
    perror("setsockopt(SO_REUSEADDR) failed");
  }


  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);
  if (bind(fd, (struct sockaddr *)&server_addr, 
      sizeof(server_addr)) < 0) {
    perror("bind");
    close(fd);
    return 1;
  }

  if (listen(fd, 5) < 0) {
    perror("listen");
    close(fd);
    return 1;
  }


  server_t server = {0};
  server.fd = fd;
  pthread_mutex_init(&server.mtx, NULL);

  pthread_t thread;
  if (pthread_create(&thread, NULL, server_loop, &server) != 0) {
    perror("pthread_create");
    close(fd);
    return 1;
  } 

  while (sleep(5) == 0) {
    for (int i = 0; i < server.count; i++) {
      process_client_packets(&server.clients[i]);
    }
    pthread_mutex_lock(&server.mtx);
    packet_t packet;
    packet.type = PACKET_TYPE_PING;
    for (int i = 0; i < server.count; i++) {
      send(server.clients[i].fd, &packet, sizeof(packet), 0);
    }
    pthread_mutex_unlock(&server.mtx);
  }

  close(fd);
  return 0;  
}
