#include "pch.h"
#include "debug.h"
#include "server.h"

int main(int argc, char *argv[]) {
  const int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    ERROR("Failed to create socket: %s", strerror(errno));
    return -1;
  }

  int flags = fcntl(fd, F_GETFL, 0);
  fcntl(fd, F_SETFL, flags | O_NONBLOCK);

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);

  int r = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
  if (r < 0) {
    ERROR("Failed to bind socket: %s", strerror(errno));
    close(fd);
    return -1;
  }

  server_t server = {0};
  server.fd = fd;

  packet_t packet;
  player_t player;

  while (1) {
    ssize_t bytes = receive_packet(&server, &packet, &player);
    if (bytes <= 0) 
      continue;

    if (packet.type == PACKET_TYPE_PING) {
      packet_t response;
      response.type = PACKET_TYPE_PING;
      response.ping.playerId = player.id;
      const struct sockaddr_in *clientAddr = &player.addr;
      bytes = send_packet(&server, clientAddr, &response);
    }

    if (packet.type == PACKET_TYPE_MOVEMENT) {
      player.x = packet.movement.x;
      player.y = packet.movement.y;
      LOG("Player %d moved to (%d, %d)", player.id, player.x, player.y);
      continue;
    }

    if (bytes < 0) {
      ERROR("Failed to send packet: %s", strerror(errno));
      continue;
    }
  }

  close(fd);
  return 0;  
}
