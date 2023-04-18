#include "composite.h"


PaneElement_::PaneElement_ (Drawable *drawable_, glm::vec3 scale_, glm::vec3 translation_)
{
  drawable = drawable_;
  scale = scale_;
  translation = translation_;
}


PaneElement_::PaneElement_ (Drawable *drawable_)
{
  drawable = drawable_;
  scale = glm::vec3(1);
  translation = glm::vec3(0);
}


CompositeElement::CompositeElement () :
  scale(glm::vec3(1)),
  translation(glm::vec3(0))
{}


void CompositeElement::draw ()
{
  for (PaneElement pe : elements)
    pe.drawable->draw();
}


bool CompositeElement::init ()
{
  bool all_ok = true;
  for (PaneElement pe : elements) {
    if (not pe.drawable->init()) {
      std::cout << "ERROR: Could not init " << pe.drawable->name() << std::endl;
      all_ok = false;
    }
  }
  return all_ok;
}


void CompositeElement::addPaneElement (PaneElement new_element)
{
  new_element.drawable->setTranslation(translation + new_element.translation);
  new_element.drawable->setScale(scale * new_element.scale);
  elements.push_back(new_element);
}


void CompositeElement::setTranslation (glm::vec3 translation)
{
  for (PaneElement pe : elements)
    pe.drawable->setTranslation(translation + pe.translation);
}


void CompositeElement::setScale (glm::vec3 scale)
{
  for (PaneElement pe : elements) {
    std::cout << "Comp scale for " << pe.drawable->name() << ": "
              << scale[0] << "," << scale[1] << "," << scale[2] << " * "
              << pe.scale[0] << "," << pe.scale[1] << "," << pe.scale[2] << std::endl;
    pe.drawable->setScale(scale * pe.scale);
  }
}
