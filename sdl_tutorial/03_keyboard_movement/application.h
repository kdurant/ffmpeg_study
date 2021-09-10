#ifndef APPLICATION_H
#define APPLICATION_H
#include "SDL.h"
#include <iostream>
#include <memory>
#include "stick_figure.h"

class Application
{
public:
    Application();
    ~Application();

    void loop();
    void update(double delta_time);
    void draw();

private:
    StickFigure  m_stick_figure;
    SDL_Window * m_window;
    SDL_Surface *m_window_surface;
    SDL_Event    m_window_event;
};
#endif  // APPLICATION_H
