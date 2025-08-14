#include <SDL3/SDL.h>
#include "pch.h"
#include "util/debug.h"
#include "network/packet.h"

#define PORT 8080

int main(int argc, char *argv[])
{
  const int fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (fd < 0) {
    ERROR("Failed to create socket: %s", strerror(errno));  
    return 1;
  }

  struct sockaddr_in addr;
  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);

  packet_t packet;
  packet.type = PACKET_TYPE_PING;
  strncpy(packet.ping.msg, "Hello, World!", MAX_STRING_SIZE);
  ssize_t bytes = sendto(fd, &packet, sizeof(packet), 0, 
    (struct sockaddr *)&addr, sizeof(addr));
  if (bytes < 0) {
    ERROR("Failed to send packet: %s", strerror(errno));
    close(fd);
    return 1;
  }

  int rc = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (rc < 0) {
    ERROR("Failed to initialize SDL: %s", SDL_GetError());
    close(fd);
    return 1;
  }

  SDL_Window *window = SDL_CreateWindow("Client", 640, 480, 0);
  if (!window) {
    ERROR("Failed to create window: %s", SDL_GetError());
    SDL_Quit();
    close(fd);
    return 1;
  }

  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
  if (!renderer) {
    ERROR("Failed to create renderer: %s", SDL_GetError());
    SDL_DestroyWindow(window);
    SDL_Quit();
    close(fd);
    return 1;
  }

  int runing = 1;
  SDL_Event event;
  while (runing) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT)
        runing = 0;
      
      if (event.type != SDL_EVENT_KEY_DOWN)
        continue;

      if (event.key.key == SDLK_Q) {
        LOG("Press Q key");
      }
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);   
  SDL_DestroyWindow(window);
  SDL_Quit();
  close(fd);

  return 0;
}
