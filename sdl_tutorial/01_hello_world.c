#include "SDL.h"
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_video.h>
#include <stdio.h>
#include <stdbool.h>
#undef main

const int WIDTH = 800, HEIGHT = 600;
int       main(void)
{
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Hello SDL world", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);

    if(NULL == window)
    {
        printf("Could not create window %s", SDL_GetError());
    }

    SDL_Event windowEvent;

    while(true)
    {
        if(SDL_PollEvent(&windowEvent))
        {
            if(SDL_QUIT == windowEvent.type)
                break;
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
