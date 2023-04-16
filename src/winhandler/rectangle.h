#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <glm/ext/matrix_transform.hpp>
#include "drawable.h"

class Rectangle : public Drawable
{
  public:
    Rectangle();
    void changeTexture (std::string texture_path, bool has_alpha = true,
                        unsigned border_mode = GL_REPEAT);
    void changeShaders (std::string shader_path);
    virtual void draw ();
    virtual bool init ();
    virtual std::string name ();
    void setName (std::string name);
    virtual void setTranslation (glm::vec3 translation);
    virtual void setScale (glm::vec3 scale);
    virtual void setRotation (float angle);
    // this is used to set a transformation previous
    // to the location of each element
    virtual void setPreTG (glm::mat4 PreTG);

  protected:
    void updateTG ();

    unsigned texture;
    bool has_alpha;
    int TGLoc, preTGLoc;
    std::string texture_path;
    std::string shader_path;
    glm::vec3 translation;
    glm::vec3 scale;
    glm::mat4 preTG;
    float angle;
    std::string rect_name;
    unsigned texture_border_mode;
};

#endif
