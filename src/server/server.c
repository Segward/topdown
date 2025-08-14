#include "pch.h"
#include "util/debug.h"
#include "network/packet.h"
#include "network/netqueue.h"

#define PORT 8080

int main(int argc, char *argv[])
{
  const int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    ERROR("Failed to create socket: %s", strerror(errno));
    return -1;
  }

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);
  int s = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
  if (s < 0) {
    ERROR("Failed to bind socket: %s", strerror(errno));
    close(fd);
    return -1;
  }
  
  netqueue_t queue;
  netqueue_init(&queue);
  pthread_t thread;
  netqueue_args_t args;
  args.fd = fd;
  args.queue = &queue;

  int rc = pthread_create(&thread, NULL, netqueue_thread, &args);
  if (rc != 0) {
    ERROR("Failed to create thread: %s", strerror(rc));
    close(fd);
    return -1;
  }

  while (1) {
    packet_t packet;
    rc = netqueue_dequeue(&queue, &packet);
    if (rc < 0) {
      ERROR("Failed to dequeue packet: %s", strerror(-rc));
      break;
    }

    LOG("Received packet: type %d", packet.type);
  }

  close(fd);
  return 0;  
}
