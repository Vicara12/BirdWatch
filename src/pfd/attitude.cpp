#include "attitude.h"
#include <glm/ext/vector_float3.hpp>
#include <string>


AttitudeInd::AttitudeInd ()
{
  background.setName("Attitude Background");
  background.changeTexture("./res/textures/pfd/PFD_background.png",
                           true, true);
  setTranslation(glm::vec3(0.f));
  setScale(glm::vec3(1.f));
  setRotation(0.f);
}


void AttitudeInd::draw ()
{
  background.draw();
}


bool AttitudeInd::init ()
{
  bool all_ok = true;
  if (not background.init()) {
    std::cout << "Could not init " << background.name() << "\n";
    all_ok = false;
  }
  return all_ok;
}


std::string AttitudeInd::name ()
{
  return "Attitude Indicator";
}


void AttitudeInd::setTranslation (glm::vec3 translation)
{
  background.setTranslation(translation);
}


void AttitudeInd::setScale (glm::vec3 scale)
{
  background.setScale(glm::vec3(scale[0],
                                scale[1]*10.f/3.f,
                                scale[2]));
}
void AttitudeInd::setRotation (float angle)
{
  background.setRotation(angle);
}
