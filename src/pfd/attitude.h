#ifndef ATTITUDE_H_
#define ATTITUDE_H_

#include "winhandler/drawable.h"
#include "winhandler/rectangle.h"
#include <glm/ext/vector_float3.hpp>

class AttitudeInd : public Drawable
{
  public:
    AttitudeInd ();
    void draw ();
    bool init ();
    std::string name ();
    void setTranslation (glm::vec3 translation);
    void setScale (glm::vec3 scale);
    void setRotation (float angle);

  private:
    Rectangle background;
};

#endif
