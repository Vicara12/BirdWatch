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
  double screen_ar = double(width)/height;
  double vp_w = vp_extremes[2]-vp_extremes[0];
  double vp_h = vp_extremes[3]-vp_extremes[1];
  double vp_ar = vp_w/vp_h;
  if (screen_ar > vp_ar) {
    vp.dimensions.w = height*vp_ar*vp.size/vp_w;
    vp.dimensions.h = height*vp.size/vp.aspect_ratio/vp_h;
    int w_vp_offset = (width - height*vp_ar)/2;
    vp.dimensions.x = w_vp_offset + height*vp_ar*(vp.center.x -
                                                  vp.size/2 -
                                                  vp_extremes[0])/vp_w;
    vp.dimensions.y = height*(vp.center.y -
                              vp.size/vp.aspect_ratio/2 -
                              vp_extremes[1])/vp_h;
  } else {
    vp.dimensions.w = width*vp.size/vp_w;
    vp.dimensions.h = width/vp_ar*vp.size/vp.aspect_ratio/vp_h;
    int h_vp_offset = (height - width/vp_ar)/2;
    vp.dimensions.x = width*(vp.center.x - vp.size/2 - vp_extremes[0])/vp_w;
    vp.dimensions.y = h_vp_offset + width/vp_ar*(vp.center.y -
                                                 vp.size/vp.aspect_ratio/2 -
                                                 vp_extremes[1])/vp_h;
  }
}


int ViewPortHandler::addViewPort (glm::vec2 center, double size, double aspect_ratio)
{
  struct ViewPort new_vp;
  new_vp.center = center;
  new_vp.size = size;
  new_vp.aspect_ratio = aspect_ratio;
  checkVpExtremes(new_vp);
  calculateViewPortDimensions(new_vp);
  view_ports.push_back(new_vp);
  return view_ports.size()-1;
}


void ViewPortHandler::checkVpExtremes (const struct ViewPort &new_vp)
{
  if (vp_extremes[0] > new_vp.center.x - new_vp.size/2)
    vp_extremes[0] = new_vp.center.x - new_vp.size/2;
  if (vp_extremes[1] > new_vp.center.y - new_vp.size/(2*new_vp.aspect_ratio))
    vp_extremes[1] = new_vp.center.y - new_vp.size/(2*new_vp.aspect_ratio);
  if (vp_extremes[2] < new_vp.center.x + new_vp.size/2)
    vp_extremes[2] = new_vp.center.x + new_vp.size/2;
  if (vp_extremes[3] < new_vp.center.y + new_vp.size/(2*new_vp.aspect_ratio))
    vp_extremes[3] = new_vp.center.y + new_vp.size/(2*new_vp.aspect_ratio);
}


void ViewPortHandler::windowResized (int width, int height)
{
  this->width = width;
  this->height = height;
  for (auto vp = view_ports.begin(); vp != view_ports.end(); vp++)
    calculateViewPortDimensions(*vp);
}


void ViewPortHandler::focusViewPort (int view_port)
{
  struct ViewPort &vp = view_ports[view_port];
  glViewport(vp.dimensions.x, vp.dimensions.y, vp.dimensions.w, vp.dimensions.h);
}
