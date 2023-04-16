#ifndef COMPOSITE_H_
#define COMPOSITE_H_

#include "drawable.h"
#include <glm/ext/matrix_transform.hpp>
#include <vector>
#include <iostream>

typedef struct PaneElement_ {
  Drawable *drawable;
  glm::vec3 scale, translation;
  std::string texture = "";
  // first element of pair must be texture path and second var name in fs
  std::vector<std::pair<std::string,std::string>> extra_textures;
  std::string shader = "";
} PaneElement;


class CompositeElement : public Drawable
{
  public:
    CompositeElement ();

    virtual void draw ();
    virtual bool init ();
    void addElement (PaneElement *new_element);
    void setTranslation (glm::vec3 translation);
    void setScale (glm::vec3 scale);

  private:
    std::vector<PaneElement*> elements;
};

 #endif
