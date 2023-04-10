#ifndef WINDOW_HANDLER_H_
#define WINDOW_HANDLER_H_

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "drawable.h"


//  typical drawing loop is
//
//  initialSetup();
//  while (windowOpen())
//    update();
//  deleteDisplay();

class WindowHandler
{
  public:

    WindowHandler (std::string window_name);
    void play ();
    void initialSetup ();
    void update ();
    void deleteDisplay ();
    bool windowOpen () const;
    void addDrawable (Drawable *drawable);
    void setRes (int width, int height);

  private:

    void drawItems ();
    void handleEvents ();

    std::string wname;
    bool window_open;
    int width, height;
    std::vector<Drawable*> window_objects;
    SDL_Window *window;
    SDL_GLContext gl_context;
};

#endif
