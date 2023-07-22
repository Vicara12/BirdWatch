#ifndef FPV_H_
#define FPV_H_

#include "winhandler/drawable.h"
#include "videoinput.h"

class FPV : public Drawable
{
  public:

    FPV();
    void setVideoSource (std::string source);
    std::string name ();
    bool init ();
    void draw ();

  private:

    void loadTexture (int width, int height, unsigned char *data);
    void setScale (glm::vec3);
    void setTranslation (glm::vec3);

    std::string video_source;
    unsigned texture;
    VideoInput video;
    int TGLoc;
};

#endif
