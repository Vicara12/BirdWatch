#ifndef VIEWPORTHANDLER_H_
#define VIEWPORTHANDLER_H_

#include <SDL2/SDL.h>
#include <glm/ext/matrix_transform.hpp>
#include <vector>

struct ViewPort {
  glm::vec2 center;
  double size;
  double aspect_ratio;
  SDL_Rect dimensions;
};


class ViewPortHandler
{
public:
  ViewPortHandler ();

  int addViewPort (glm::vec2 center, double size, double aspect_ratio);
  void windowResized (int width, int height);
  void focusViewPort (int view_port);
  int getWidth ();
  int getHeight ();

private:

  void calculateViewPortDimensions (struct ViewPort &vp);

  std::vector <struct ViewPort> view_ports;
  int width = 0, height = 0;
  double vp_extremes[4] = {0, 0, 0, 0}; // min x, min y, max x, max y
};

#endif
