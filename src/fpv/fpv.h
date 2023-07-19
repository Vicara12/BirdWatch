#ifndef FPV_H_
#define FPV_H_

#include "winhandler/drawable.h"

class FPV : public Drawable
{
  public:

    FPV();
    void setVideoSource (std::string source);
    std::string name ();
    bool init ();
    void draw ();

  private:

  void setScale (glm::vec3);
  void setTranslation (glm::vec3);

  std::string video_source;
};

#endif
