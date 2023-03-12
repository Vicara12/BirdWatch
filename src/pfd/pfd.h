#ifndef PFD_H_
#define PFD_H_

#include "winhandler/drawable.h"

class PFD : public Drawable
{
  public:

    PFD();
    bool init ();
    void draw ();

  private:
    unsigned shaders_program;
    unsigned vao_square;
    unsigned texture;
};

#endif
