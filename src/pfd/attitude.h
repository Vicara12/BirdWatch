#ifndef ATTITUDE_H_
#define ATTITUDE_H_

#include "winhandler/composite.h"
#include "winhandler/rectangle.h"
#include <glm/ext/vector_float3.hpp>

class AttitudeInd : public CompositeElement
{
  public:
    AttitudeInd ();
    void draw ();
    bool init ();
    std::string name ();
    void setPitchRoll (float pitch, float roll);

  private:
    void loadTextureTransformMat ();
    void angleIndicatorLoadPreTG();

    Rectangle attitude_ind, angle_ind;
    float pitch, roll;
    unsigned texTransLoc;
    unsigned border, statics;
};

#endif
