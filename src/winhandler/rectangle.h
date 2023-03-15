#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "drawable.h"

class Rectangle : public Drawable
{
  public:
    Rectangle();
    void changeTexture (std::string texture_path);
    void changeShaders (std::string shader_path);
    virtual void draw ();
    virtual bool init ();
    virtual std::string name () = 0;

  protected:
    unsigned texture;
    std::string texture_path;
    std::string shader_path;
};

#endif
