#include "pfd.h"

PFD::PFD ()
{
}

bool PFD::init()
{
  Drawable::generateSquare(VAO, EBO);
  if (not Drawable::loadProgram("./res/shaders/pfd", shader_program) or
      (not Drawable::loadTexture("./res/textures/pfd/awesomeface.png", texture, true, false)))
    return false;
  glUniform1i(glGetUniformLocation(shader_program, "texture0"), 0);
  return true;
}

std::string PFD::name ()
{
  return "Primary Flight Display";
}

void PFD::draw()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  Drawable::draw();
}
