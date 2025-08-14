#include "../shared/network/netqueue.h"

#define PORT 8080

int main(int argc, char *argv[])
{
  const int fd = socket(AF_INET, SOCK_DGRAM, 0);
  ASSERT_FALSE(fd, -1, "Failed to create socket");
  LOG("Socket created successfully");

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = INADDR_ANY;
  addr.sin_port = htons(PORT);
  int s = bind(fd, (struct sockaddr *)&addr, sizeof(addr));
  ASSERT(s, 0, "Failed to bind socket");
  LOG("Socket bound successfully");

  netqueue_t queue;
  netqueue_init(&queue);
  pthread_t thread;
  netqueue_args_t args;
  args.fd = fd;
  args.queue = &queue;

  int rc = pthread_create(&thread, NULL, netqueue_thread, &args);
  ASSERT(rc, 0, "Failed to create thread");

  while (1) {
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
      packet_t packet;
      rc = netqueue_dequeue(&queue, &packet);
      ASSERT(rc, 0, "Failed to dequeue packet");
      LOG("Received packet: type %d", packet.type);
    }
  }

  close(fd);
  return 0;  
}
