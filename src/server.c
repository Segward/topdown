#include <sqlite3.h>
#include "pch.h"
#include "network.h"

int main(int argc, char *argv[]) {
  client_t client = {0};
  network_server_init(&client);

  while (1) {
    sleep(5);
  }

  return 0;  
}
