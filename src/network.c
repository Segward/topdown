#include "pch.h"
#include "network.h"

void *network_server_loop(void *args) {
  client_t *client = (client_t *)args;
  network_t *network = &client->network;
  channel_t *channel = &network->channel;
 
  struct sockaddr_in c_addr;
  socklen_t addr_len = sizeof(c_addr);

  while (1) {
    int c_fd = accept(channel->fd, (struct sockaddr *)&c_addr, &addr_len);
    if (c_fd < 0) {
      perror("accept");
      continue;
    }

    if (network->connection_count >= MAX_CONNECTIONS) {
      close(c_fd);
      continue;
    }

    int index = network->connection_count++;
    connection_t *connection = &network->connections[index];
    channel_t *c_channel = &connection->channel;
    c_channel->fd = c_fd;

    pthread_mutex_init(&c_channel->mutex, NULL);
    if (pthread_create(&c_channel->thread, NULL, channel_loop, c_channel) != 0) {
      perror("pthread_create");
      close(c_fd);
    }

    pthread_detach(c_channel->thread);
    printf("client joined\n");
    fflush(stdout);
  }

  return NULL;
}

int network_server_init(client_t *client) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("socket");
    return 1;
  }

  int opt = 1;
  if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    close(fd);
    return 1;
  }

  struct sockaddr_in s_addr;
  s_addr.sin_family = AF_INET;
  s_addr.sin_addr.s_addr = INADDR_ANY;
  s_addr.sin_port = htons(SERVER_PORT);
  if (bind(fd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0) {
    perror("bind");
    close(fd);
    return 1;
  }

  if (listen(fd, 5) < 0) {
    perror("listen");
    close(fd);
    return 1;
  }

  network_t *network = &client->network;
  channel_t *channel = &network->channel;
  channel->fd = fd;

  pthread_mutex_init(&channel->mutex, NULL);
  if (pthread_create(&channel->thread, NULL, network_server_loop, client) != 0) {
    perror("pthread_create");
    close(fd);
    return 1;
  }

  pthread_detach(channel->thread);
  return 0;
}

int network_client_init(client_t *client) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("socket");
    return 1;
  }

  struct sockaddr_in s_addr;
  s_addr.sin_family = AF_INET;
  s_addr.sin_port = htons(SERVER_PORT);

  if (inet_pton(AF_INET, SERVER_IP, &s_addr.sin_addr) <= 0) {
    perror("inet_pton");
    close(fd);
    return 1;
  } 

  if (connect(fd, (struct sockaddr *)&s_addr, sizeof(s_addr)) < 0) {
    perror("connect");
    close(fd);
    return 1;
  }

  network_t *network = &client->network;
  channel_t *channel = &network->channel;
  channel->fd = fd;

  pthread_mutex_init(&channel->mutex, NULL);
  if (pthread_create(&channel->thread, NULL, channel_loop, channel) != 0) {
    perror("pthread_create");
    close(fd);
    return 1;
  }

  pthread_detach(channel->thread);
  return 0;
}










