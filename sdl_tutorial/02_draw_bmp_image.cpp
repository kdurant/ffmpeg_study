#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <iostream>
#include <SDL.h>
#undef main

const int WIDTH = 800, HEIGHT = 600;

int main(void)
{
    SDL_Surface* imageSurface  = NULL;
    SDL_Surface* windowSurface = NULL;

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
        std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << "\n";

    SDL_Window* window = SDL_CreateWindow("Hello SDL world", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    windowSurface      = SDL_GetWindowSurface(window);

    if(NULL == window)
    {
        std::cout << "Could not create window: " << SDL_GetError() << "\n";
        return EXIT_FAILURE;
    }

    SDL_Event windowEvent;
    imageSurface = SDL_LoadBMP("../sdl_tutorial/hello_world.bmp");
    if(imageSurface == NULL)
    {
        std::cout << "SDL could not load image! SDL Error: " << SDL_GetError() << std::endl;
    }

    while(true)
    {
        if(SDL_PollEvent(&windowEvent))
        {
            if(SDL_QUIT == windowEvent.type)
                break;
        }

        SDL_BlitSurface(imageSurface, NULL, windowSurface, NULL);
        SDL_UpdateWindowSurface(window);
    }

    SDL_FreeSurface(imageSurface);
    SDL_FreeSurface(windowSurface);

    imageSurface  = NULL;
    windowSurface = NULL;
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
