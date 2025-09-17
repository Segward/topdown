#include <SDL3/SDL.h>
#include "pch.h"
#include "network.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("%s <id>\n", argv[0]);
    return 1;
  }

  for (int i = 0; argv[1][i] != '\0'; i++) {
    if (!isdigit(argv[1][i])) {
      printf("id must be a positive integer\n");
      return 1;
    }
  }

  int id = atoi(argv[1]);
  if (id <= 0) {
    printf("id must be a positive integer\n");
    return 1;
  }

  client_t client = {0};
  network_client_init(&client);

  while (1) {
    sleep(5);
  } 

  return 0;
}
