#include "windowhandler.h"
#include <iostream>


WindowHandler::WindowHandler (std::string window_name)
  : wname(window_name), width(1200), height(600)
{

}

void WindowHandler::play()
{
  initialSetup();
  while (window_open)
    update();
  deleteDisplay();
}

void WindowHandler::setRes (int width, int height)
{
  this->width = width;
  this->height = height;
}

void WindowHandler::initialSetup ()
{
  SDL_Init(SDL_INIT_EVERYTHING);

  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

  window = SDL_CreateWindow(wname.c_str(), SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, width, height,
                            SDL_WINDOW_OPENGL);

  gl_context = SDL_GL_CreateContext(window);
  if (glewInit() != GLEW_OK)
    std::cout << "ERROR: could not initialize window\n";
  glClearColor(0.1f,0.1f,0.1f,1.f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  window_open = true;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  for (Drawable *drawable : window_objects)
    if (not drawable->init())
      std::cout << "Could not init the panel " << drawable->name() << std::endl;
}

void WindowHandler::update ()
{
  SDL_GL_SwapWindow(window);
  drawItems();
  handleEvents();
}

void WindowHandler::drawItems ()
{
  glClear(GL_COLOR_BUFFER_BIT);
  for (Drawable *drawable : window_objects)
    drawable->draw();
}

void WindowHandler::addDrawable (Drawable *drawable)
{
  window_objects.push_back(drawable);
}

void WindowHandler::handleEvents ()
{
  SDL_Event e;

  while (SDL_PollEvent(&e)) {
    switch(e.type) {
      case SDL_QUIT:
        window_open = false;
        break;
      default:
        break;
    }
  }
}

void WindowHandler::deleteDisplay ()
{
  SDL_GL_DeleteContext(gl_context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}
