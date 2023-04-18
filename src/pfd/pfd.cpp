#include "pfd.h"
#include <glm/detail/qualifier.hpp>
#include <glm/ext/vector_float3.hpp>

PFD::PFD ()
{
  addPaneElement(PaneElement(&background, glm::vec3(2,2,1), glm::vec3(0)));
  addPaneElement(PaneElement(&attitude_ind));
  addPaneElement(PaneElement(&yaw_ind, glm::vec3(1.5, 1.5, 1), glm::vec3(0, -0.85, 0)));
  background.setName("background");
}


std::string PFD::name ()
{
  return "Primary Flight Display";
}


void PFD::setYPR (float yaw, float pitch, float roll)
{
  attitude_ind.setPitchRoll(pitch, roll);
  yaw_ind.setYaw(yaw);
}
