#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include <GL/glew.h>
#include <iostream>

class Drawable
{
  public:
    Drawable ();

    virtual void draw () = 0;
    virtual bool init () = 0;
    virtual std::string name () = 0;

    static bool loadShader (GLenum shader_type, const std::string &path, unsigned &shader);
    static bool loadProgram (std::string path, unsigned &program);
    static bool loadTexture (std::string path, unsigned &texture, bool has_alpha, bool flip);
    static void generateSquare (unsigned &VAO);

  private:
};

#endif
