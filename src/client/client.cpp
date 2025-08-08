#include <SDL3/SDL.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

int main(int argc, char *argv[])
{
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in server_addr = {};
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(8080);
  inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);
  connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

  SDL_Init(SDL_INIT_VIDEO);
  SDL_Window *window = SDL_CreateWindow("topdown", 800, 600, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);

  SDL_Event event;
  bool running = true;

  while (running)
  {
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_EVENT_QUIT)
      {
        running = false;
      }
      else if (event.type == SDL_EVENT_KEY_DOWN)
      {
        if (event.key.key == SDLK_W)
        {
          const char *msg = "Request from SDL client\n";
          send(sock, msg, strlen(msg), 0);
        }
        else if (event.key.key == SDLK_ESCAPE)
        {
          running = false;
          std::cout << "Exiting client application" << std::endl;
        }
      }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
  }

  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
