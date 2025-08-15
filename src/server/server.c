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

  while (1) {

  }

  close(fd);
  return 0;  
}
