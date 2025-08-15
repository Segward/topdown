#include <SDL3/SDL.h>
#include "pch.h"
#include "debug.h"
#include "client.h"

int main(int argc, char *argv[]) {
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

  int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  if (result < 0) {
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

  player_t player;
  player.x = 0;
  player.y = 0; 

  client_t client;
  client.fd = fd;
  client.addr = addr;
  client.player = player;

  packet_t packet;
  packet.type = PACKET_TYPE_PING;
  ssize_t bytes = send_packet(&client, &packet);
  if (bytes < 0) {
    ERROR("Failed to send packet: %s", strerror(errno));
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    close(fd);
    return 1;
  }

  bytes = receive_packet(&client, &packet);
  if (bytes < 0) {
    ERROR("Failed to receive packet: %s", strerror(errno));
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    close(fd);
    return 1;
  }

  client.player.id = packet.ping.playerId;
  LOG("Player ID: %d", client.player.id);

  int running = 1;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT)
        running = 0;
      
      if (event.type != SDL_EVENT_KEY_DOWN)
        continue;

      int movement = 0;
      if (event.key.key == SDLK_W) {
        client.player.y -= 5;
        movement = 1;
      } else if (event.key.key == SDLK_S) {
        client.player.y += 5;
        movement = 1;
      } else if (event.key.key == SDLK_A) {
        client.player.x -= 5;
        movement = 1;
      } else if (event.key.key == SDLK_D) {
        client.player.x += 5;
        movement = 1;
      }

      if (movement) {
        packet.type = PACKET_TYPE_MOVEMENT;
        packet.movement.playerId = client.player.id;
        packet.movement.x = client.player.x;
        packet.movement.y = client.player.y;
        bytes = send_packet(&client, &packet);
      }

      if (bytes < 0) {
        ERROR("Failed to send movement packet: %s", strerror(errno));
        running = 0;
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
