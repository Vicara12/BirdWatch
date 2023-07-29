#ifndef FPV_H_
#define FPV_H_

#include "winhandler/drawable.h"
#include "videoinput.h"
#include <vector>

typedef std::pair<std::string,std::vector<std::string>> Source;

class FPV : public Drawable
{
  public:

    FPV();
    void setVideoSource (std::string source);
    std::string name ();
    bool preInitVideo ();
    bool init ();
    void draw ();
    double getAR () const;

  private:

    void cannotOpenVideoSource ();
    std::vector<Source> listCameras ();
    void genTexture ();
    void loadTexture (unsigned char *data);
    void setScale (glm::vec3);
    void setTranslation (glm::vec3);

    bool video_inited;
    std::string video_source;
    unsigned texture;
    VideoInput video;
    int TGLoc;
    int width, height;
};

#endif
