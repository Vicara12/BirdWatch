#include "pfd.h"
#include <glm/ext/vector_float3.hpp>

PFD::PFD ()
{
}

void PFD::draw ()
{
  //float pos = (int(clock()/1e3)%100)/100.f;
  a.setTranslation(glm::vec3(0.5f, 0.f, 0.f));
  b.setTranslation(glm::vec3(-0.5f, 0.f, 0.f));
  a.draw();
  b.draw();
}

bool PFD::init ()
{
  a.changeTexture("./res/textures/awesomeface.png");
  return a.init() and b.init();
}

std::string PFD::name ()
{
  return "Primary Flight Display";
}
