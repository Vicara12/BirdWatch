#include "fpv.h"

FPV::FPV () {}

void FPV::setScale (glm::vec3)
{
  //
}

void FPV::setTranslation (glm::vec3)
{
  //
}

void FPV::setVideoSource (std::string source)
{
  this->video_source = source;
}

std::string FPV::name ()
{
  return "FPV stream";
}

bool FPV::init ()
{
  // TODO
  return true;
}


void FPV::draw ()
{
  // TODO
}
