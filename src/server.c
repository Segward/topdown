#include <sqlite3.h>
#include "pch.h"
#include "network.h"

int main(int argc, char *argv[]) {
  network_t network = {0};
  network_server_init(&network);

  while (1) {
    sleep(1);
    network_free_inactive(&network);
  }

  return 0;  
}
