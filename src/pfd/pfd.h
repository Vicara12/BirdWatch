#ifndef PFD_H_
#define PFD_H_

#include "winhandler/composite.h"
#include "winhandler/rectangle.h"
#include "attitude.h"
#include "yaw.h"
#include "barindicator.h"

class PFD : public CompositeElement
{
  public:

    PFD();
    std::string name ();
    void setYPR (float yaw, float pitch, float roll);
    void setDataLinkStatus (bool data_received);

  private:

    bool data_received;
    AttitudeInd attitude_ind;
    YawIndicator yaw_ind;
    Rectangle background;
    BarIndicator altimeter;
};

#endif
