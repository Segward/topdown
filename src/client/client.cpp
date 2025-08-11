#include "../shared/network.hpp"
#include <SDL3/SDL.h>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    network::client client("127.0.0.1", 8080);
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
            if (event.type == SDL_EVENT_KEY_DOWN)
            {
                if (event.key.key == SDLK_Q)
                {

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
