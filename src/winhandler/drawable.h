#ifndef DRAWABLE_H_
#define DRAWABLE_H_

#include <GL/glew.h>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>

class Drawable
{
  public:
    Drawable ();

    virtual void draw ();
    virtual bool init () = 0;
    virtual std::string name () = 0;
    virtual void setScale (glm::vec3 scale) = 0;
    virtual void setTranslation (glm::vec3 translation) = 0;
    unsigned getShaderProgram () const;

  protected:
    static bool loadShader (GLenum shader_type, const std::string &path, unsigned &shader);
    static bool loadProgram (std::string path, unsigned &program);
    static bool loadTexture (std::string path, unsigned &texture, bool has_alpha,
                             unsigned border_mode);
    static void generateSquare (unsigned &VAO, unsigned &EBO);

    unsigned VAO, EBO;
    unsigned shader_program;
};

#endif
