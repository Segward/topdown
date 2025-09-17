#include "pch.h"
#include "network.h"
#include "player.h"

int network_free_connection(network_t *network) {
  for (int i = 0; i < MAX_CONNECTIONS; i++) {
    connection_t *connection = &network->connections[i];
    channel_t *channel = &connection->channel;
    if (channel->active == 0 && channel->free == 0)
      return i;
  }
  return -1;
}

int network_free_inactive(network_t *network) {
  for (int i = 0; i < MAX_CONNECTIONS; i++) {
    connection_t *connection = &network->connections[i];
    channel_t *channel = &connection->channel;
    if (!(channel->active == 0 && channel->free == 1))
      continue;

    pthread_mutex_destroy(&channel->mutex);
    channel->free = 0;

    player_t *player = connection->player;
    if (player != NULL)
      memset(player, 0, sizeof(player_t));

    memset(connection, 0, sizeof(connection_t));
    printf("client disconnected\n");
  }

  return 0;
}

void *network_server_loop(void *args) {
  network_t *network = (network_t *)args;
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

    int index = network_free_connection(network);
    if (index < 0) {
      close(c_fd);
      continue;
    }

    network->connection_count++;
    connection_t *connection = &network->connections[index];
    channel_t *c_channel = &connection->channel;
    c_channel->fd = c_fd;
    c_channel->active = 1;
    c_channel->free = 1;

    pthread_mutex_init(&c_channel->mutex, NULL);
    if (pthread_create(&c_channel->thread, NULL, channel_loop, c_channel) != 0) {
      perror("pthread_create");
      close(c_fd);
    }

    printf("client connected\n");
    pthread_detach(c_channel->thread);
    fflush(stdout);
  }

  return NULL;
}

int network_server_init(network_t *network) {
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

  channel_t *channel = &network->channel;
  channel->fd = fd;

  pthread_mutex_init(&channel->mutex, NULL);
  if (pthread_create(&channel->thread, NULL, network_server_loop, network) != 0) {
    perror("pthread_create");
    close(fd);
    return 1;
  }

  pthread_detach(channel->thread);
  return 0;
}

int network_client_init(network_t *network) {
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
