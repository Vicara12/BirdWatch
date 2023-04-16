#include "composite.h"


CompositeElement::CompositeElement ()
{
}


void CompositeElement::draw ()
{
  for (PaneElement *pe : elements)
    pe->drawable->draw();
}


bool CompositeElement::init ()
{
  bool all_ok = true;
  for (PaneElement *pe : elements) {
    if (not pe->drawable->init()) {
      std::cout << "ERROR: Could not init " << pe->drawable->name() << std::endl;
      all_ok = false;
    }
  }
  return all_ok;
}


void CompositeElement::addElement (PaneElement *new_element)
{
  elements.push_back(new_element);
}


void CompositeElement::setTranslation (glm::vec3 translation)
{
  for (PaneElement *pe : elements)
    pe->drawable->setScale(translation + pe->translation);
}


void CompositeElement::setScale (glm::vec3 scale)
{
  for (PaneElement *pe : elements)
    pe->drawable->setTranslation(scale * pe->scale);
}

