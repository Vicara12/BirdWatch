#include "pfd.h"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>

PFD::PFD ()
{
  setScale(glm::vec3(1.f, 1.f, 1.f));
  setTranslation(glm::vec3(0.f, 0.f, 0.f));
  setRotation(0.f);
}

void PFD::draw ()
{
  background.draw();
  attitude_ind.draw();
  yaw_ind.draw();
}

bool PFD::init ()
{
  bool all_ok = true;
  if (not attitude_ind.init()) {
    std::cout << "Could not init " << attitude_ind.name() << "\n";
    all_ok = false;
  }
  if (not background.init()) {
    std::cout << "Could not init " << background.name() << "\n";
    all_ok = false;
  }
  if (not yaw_ind.init()) {
    std::cout << "Could not init " << yaw_ind.name() << "\n";
    all_ok = false;
  }
  return all_ok;
}


std::string PFD::name ()
{
  return "Primary Flight Display";
}


void PFD::setTranslation (glm::vec3 translation)
{
  attitude_ind.setTranslation(translation);
  background.setTranslation(translation);
  yaw_ind.setTranslation(glm::vec3(translation[0]+0,
                                   translation[1]-0.85,
                                   translation[2]));
}


void PFD::setScale (glm::vec3 scale)
{
  attitude_ind.setScale(glm::vec3(scale[0]*1,
                                  scale[1]*1,
                                  scale[2]*1));
  background.setScale(glm::vec3(scale[0]*2,
                                scale[1]*2,
                                scale[2]*1));
  yaw_ind.setScale(glm::vec3(scale[0]*1.5,
                             scale[1]*1.5,
                             scale[2]*1));
}


void PFD::setRotation (float angle)
{
  attitude_ind.setRotation(angle);
  background.setRotation(angle);
  yaw_ind.setRotation(angle);
}


void PFD::setYPR (float yaw, float pitch, float roll)
{
  attitude_ind.setPitchRoll(pitch, roll);
  yaw_ind.setYaw(yaw);
}
