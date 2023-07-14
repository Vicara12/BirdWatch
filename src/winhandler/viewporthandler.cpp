#include "viewporthandler.h"
#include <GL/glew.h>
#include <iostream>


ViewPortHandler::ViewPortHandler ()
{
}

int ViewPortHandler::getWidth ()
{
  return width;
}

int ViewPortHandler::getHeight ()
{
  return height;
}

void ViewPortHandler::calculateViewPortDimensions (struct ViewPort &vp)
{
  if (width > height) {
    vp.dimensions.w = height*vp.size;
    vp.dimensions.h = height*vp.size/(vp.aspect_ratio);
  } else {
    vp.dimensions.w = width*vp.size;
    vp.dimensions.h = width*vp.size/(vp.aspect_ratio);
  }
  vp.dimensions.x = width*vp.center.x - vp.dimensions.w/2;
  vp.dimensions.y = height*vp.center.y - vp.dimensions.h/2;
}


int ViewPortHandler::addFullScreenViewPort ()
{
  struct ViewPort new_vp;
  new_vp.full_screen = true;
  view_ports.push_back(new_vp);
  return view_ports.size()-1;
}


int ViewPortHandler::addViewPort (glm::vec2 center, double size, double aspect_ratio)
{
  struct ViewPort new_vp;
  new_vp.full_screen = false;
  new_vp.center = center;
  new_vp.size = size;
  new_vp.aspect_ratio = aspect_ratio;
  calculateViewPortDimensions(new_vp);
  view_ports.push_back(new_vp);
  return view_ports.size()-1;
}


void ViewPortHandler::windowResized (int width, int height)
{
  this->width = width;
  this->height = height;
  for (auto vp = view_ports.begin(); vp != view_ports.end(); vp++)
    if (not vp->full_screen)
      calculateViewPortDimensions(*vp);
}


void ViewPortHandler::focusViewPort (int view_port)
{
  struct ViewPort &vp = view_ports[view_port];
  if (view_ports[view_port].full_screen)
    glViewport(0, 0, width, height);
  else
    glViewport(vp.dimensions.x, vp.dimensions.y, vp.dimensions.w, vp.dimensions.h);
}
