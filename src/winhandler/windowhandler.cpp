#include "windowhandler.h"


WindowHandler::WindowHandler (std::string window_name)
  : wname(window_name), width(1800), height(900)
{

}

void WindowHandler::play()
{
  initialSetup();
  while (window_open)
    update();
  deleteDisplay();
}


bool WindowHandler::windowOpen () const
{
  return window_open;
}

void WindowHandler::setRes (int width, int height)
{
  this->width = width;
  this->height = height;
}

bool WindowHandler::initialSetup ()
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
  SDL_SetWindowResizable(window, SDL_TRUE);

  if (glewInit() != GLEW_OK) {
    std::cout << "ERROR: could not initialize window\n";
    return false;
  }
  glClearColor(0.1f,0.1f,0.1f,1.f);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  window_open = true;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

  for (std::pair <int,Drawable*> drawable : window_objects)
    if (not drawable.second->init()) {
      std::cout << "ERROR: could not init the panel " << drawable.second->name() << std::endl;
      return false;
    }
  return true;
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
  int w, h;
  SDL_GetWindowSize(window, &w, &h);
  if (vp_handler.getWidth() != w or vp_handler.getHeight() != h) {
    width = w;
    height = h;
    vp_handler.windowResized(width, height);
  }
  for (std::pair <int,Drawable*> drawable : window_objects) {
    vp_handler.focusViewPort(drawable.first);
    drawable.second->draw();
  }
}

void WindowHandler::addDrawable (Drawable *drawable, glm::vec2 center,
                                 double size, double aspect_ratio)
{
  int vp_number = vp_handler.addViewPort(center, size, aspect_ratio);
  window_objects.push_back(std::make_pair(vp_number, drawable));
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

bool WindowHandler::setViewPortCenter (int vp_id, glm::vec2 center)
{
  return vp_handler.changeCenter(vp_id, center);
}

bool WindowHandler::setViewPortSize (int vp_id, double size)
{
  return vp_handler.changeSize(vp_id, size);
}

bool WindowHandler::setViewPortAR (int vp_id, double ar)
{
  return vp_handler.changeAR(vp_id, ar);
}
