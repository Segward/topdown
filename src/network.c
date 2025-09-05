#include "pch.h"
#include "client.h"
#include "network.h"

void *network_connection_loop(void *args) {
  connection_t *connection = (connection_t *)args;


  return 0;
}

void *network_server_loop(void *args) {
  client_t *client = (client_t *)args;
  int connected_fd;
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);

  while (1) {
    connected_fd = accept(client->network.fd, (struct sockaddr *)&client_addr, &client_addr_len);
    if (connected_fd < 0) {
      perror("accept");
      continue;
    }

    connection_t *connection = malloc(sizeof(connection_t));
    if (!connection) {
      perror("malloc");
      close(connected_fd);
      continue;
    }

    connection->fd = connected_fd;
    connection->id = client->connection_count++;
    pthread_t thread = connection->thread;
    pthread_mutex_init(&connection->lock, NULL);
    if (pthread_create(&thread, NULL, network_connection_loop, connection) != 0) {
      perror("pthread_create");
      close(connected_fd);
      free(connection);
      continue;
    }

    pthread_detach(thread);
  }

  return 0;
}

void *network_client_loop(void *args) {
  client_t *client = (client_t *)args;
  packet_t packet;

  while (1) {
    ssize_t bytes = recv(client->network.fd, &packet, sizeof(packet), 0);
    if (bytes == 0) 
      continue;

    if (bytes < 0)
      break;

    
    printf("Received packet\n");
  }

  return 0;
}

int network_server_init(client_t *client) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("socket");
    return -1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(SERVER_PORT);
  if (bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("bind");
    close(fd);
    return -1;
  }

  if (listen(fd, MAX_CONNECTIONS) < 0) {
    perror("listen");
    close(fd);
    return -1;
  }

  client->network.fd = fd;
  pthread_t thread = client->network.thread;
  pthread_mutex_init(&client->network.send_lock, NULL);
  if (pthread_create(&thread, NULL, network_server_loop, client) != 0) {
    perror("pthread_create");
    close(fd);
    return -1;
  }

  pthread_detach(thread);
  return 0;
}

int network_client_init(client_t *client) {
  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    perror("socket");
    return -1;
  }

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(SERVER_PORT);
  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
    perror("inet_pton");
    close(fd);
    return -1;
  }

  if (connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    perror("connect");
    close(fd);
    return -1;
  }

  client->network.fd = fd;
  pthread_t thread = client->network.thread;
  pthread_mutex_init(&client->network.send_lock, NULL);
  if (pthread_create(&thread, NULL, network_client_loop, client) != 0) {
    perror("pthread_create");
    close(fd);
    return -1;
  }
  
  pthread_detach(thread);
  return 0;
}
