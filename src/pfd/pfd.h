#ifndef PFD_H_
#define PFD_H_

#include "winhandler/drawable.h"
#include "winhandler/rectangle.h"
#include "attitude.h"
#include "yaw.h"
#include "barindicator.h"
#include <glm/ext/vector_float3.hpp>

class PFD : public Drawable
{
  public:

    PFD();
    void draw ();
    bool init ();
    std::string name ();
    void setTranslation (glm::vec3 translation);
    void setScale (glm::vec3 scale);
    void setRotation (float angle);
    void setYPR (float yaw, float pitch, float roll);

  private:

    AttitudeInd attitude_ind;
    YawIndicator yaw_ind;
    Rectangle background;
    BarIndicator altimeter;
};

#endif
