#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <glm/ext/matrix_transform.hpp>
#include "drawable.h"

class Rectangle : public Drawable
{
  public:
    Rectangle();
    void changeTexture (std::string texture_path, bool has_alpha = true, bool flip_texture = true);
    void changeShaders (std::string shader_path);
    virtual void draw ();
    virtual bool init ();
    virtual std::string name ();
    void setName (std::string name);
    virtual void setTranslation (glm::vec3 translation);
    virtual void setScale (glm::vec3 scale);
    virtual void setRotation (float angle);

  protected:
    void updateTG ();

    unsigned texture;
    bool has_alpha, flip_texture;
    unsigned TGLoc;
    std::string texture_path;
    std::string shader_path;
    glm::vec3 translation;
    glm::vec3 scale;
    float angle;
    std::string rect_name;
};

#endif
