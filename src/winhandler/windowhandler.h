#ifndef WINDOW_HANDLER_H_
#define WINDOW_HANDLER_H_

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>


class WindowHandler
{
  public:

    WindowHandler (std::string window_name);
    void play ();
    void setRes (int width, int height);

  private:

    void initialSetup ();
    void update ();
    void drawItems ();
    void handleEvents ();
    void deleteDisplay ();

    std::string wname;
    bool window_open;
    int width, height;
    SDL_Window *window;
    SDL_GLContext gl_context;
};

#endif
