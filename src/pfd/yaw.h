#ifndef YAW_H_
#define YAW_H_

#include "winhandler/composite.h"
#include "winhandler/rectangle.h"

class YawIndicator : public CompositeElement
{
  public:
    YawIndicator ();

    bool init ();
    void draw ();
    std::string name ();
    void setYaw (float yaw);

  private:

    void updateTextureTransform ();

    Rectangle compass, indicator;
    float yaw;
    int texTransLoc;
};

#endif
