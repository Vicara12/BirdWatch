#ifndef PFD_H_
#define PFD_H_

#include "winhandler/drawable.h"
#include "winhandler/rectangle.h"

class PFD : public Drawable
{
  public:

    PFD();
    void draw ();
    bool init ();
    std::string name ();

  private:

    Rectangle a,b;
};

#endif
