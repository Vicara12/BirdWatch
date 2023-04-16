#include "barindicator.h"

BarIndicator::BarIndicator ()
{
}


void BarIndicator::draw ()
{
  background.draw();
  //number_ind.draw();
}


bool BarIndicator::init ()
{
  bool all_ok = false;
  if (not background.init()) {
    std::cout << "Could not init " << background.name() << "\n";
    all_ok = false;
  }
  if (not number_ind.init()) {
    std::cout << "Could not init " << number_ind.name() << "\n";
    all_ok = false;
  }
  return all_ok;
}


std::string BarIndicator::name ()
{
  return "Bar Indicator";
}


void BarIndicator::setScale (glm::vec3 scale)
{
  background.setScale(scale);
  number_ind.setScale(scale);
}


void BarIndicator::setTranslation (glm::vec3 translation)
{
  background.setTranslation(translation);
  number_ind.setTranslation(translation);
}
