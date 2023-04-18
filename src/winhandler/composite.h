#ifndef COMPOSITE_H_
#define COMPOSITE_H_

#include "drawable.h"
#include <glm/ext/matrix_transform.hpp>
#include <vector>
#include <iostream>

typedef struct PaneElement_ {
  Drawable *drawable;
  glm::vec3 scale = glm::vec3(1);
  glm::vec3 translation = glm::vec3(0);

  PaneElement_(Drawable *drawable_);
  PaneElement_(Drawable *drawable_, glm::vec3 scale_, glm::vec3 translation_);
} PaneElement;


class CompositeElement : public Drawable
{
  public:
    CompositeElement ();

    virtual void draw ();
    virtual bool init ();
    void addPaneElement (PaneElement new_element);
    void setTranslation (glm::vec3 translation);
    void setScale (glm::vec3 scale);

  private:
    glm::vec3 scale, translation;
    std::vector<PaneElement> elements;
};

 #endif
