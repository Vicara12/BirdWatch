#ifndef YAW_H_
#define YAW_H_

#include "winhandler/drawable.h"
#include "winhandler/rectangle.h"

class YawIndicator : public Drawable
{
  public:
    YawIndicator ();

    void draw ();
    bool init ();
    std::string name ();
    void setTranslation (glm::vec3 translation);
    void setScale (glm::vec3 scale);
    void setRotation (float angle);
    void setYaw (float yaw);

  private:

    void updateTextureTransform ();

    Rectangle compass, indicator;
    float yaw;
    int texTransLoc;
};

#endif
