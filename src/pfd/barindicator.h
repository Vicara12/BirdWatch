#ifndef BARINDICATOR_H_
#define BARINDICATOR_H_

#include "winhandler/drawable.h"
#include "winhandler/rectangle.h"

class BarIndicator : public Drawable
{
  public:
    BarIndicator ();
    virtual void draw ();
    virtual bool init ();
    virtual std::string name ();
    virtual void setScale (glm::vec3 scale);
    virtual void setTranslation (glm::vec3 translation);

  private:

    Rectangle background;
    Rectangle number_ind;
};

#endif
