#ifndef WINDOW_HANDLER_H_
#define WINDOW_HANDLER_H_

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <vector>
#include "drawable.h"
#include "viewporthandler.h"


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
    bool initialSetup ();
    void update ();
    void deleteDisplay ();
    bool windowOpen () const;
    void addDrawable (Drawable *drawable, glm::vec2 center,
                      double size, double aspect_ratio);
    void setRes (int width, int height);

    bool setViewPortCenter (int vp_id, glm::vec2 center);
    bool setViewPortSize   (int vp_id, double size);
    bool setViewPortAR     (int vp_id, double ar);

  private:

    void drawItems ();
    void handleEvents ();

    std::string wname;
    bool window_open;
    int width, height;
    std::vector<std::pair<int, Drawable*>> window_objects; // viewport and drawable
    SDL_Window *window;
    SDL_GLContext gl_context;
    SDL_Renderer *renderer;
    ViewPortHandler vp_handler;
};

#endif
