#include "pfd.h"


PFD::PFD () {}

bool PFD::init()
{
  if (not Drawable::loadProgram("./res/shaders/pfd", shaders_program))
      //not Drawable::loadTexture("./res/textures/pfd/awesomeface.png", texture, true, false))
    return false;
  Drawable::generateSquare(vao_square);
  //glUniform1i(glGetUniformLocation(shaders_program, "texture0"), 0);
  return true;
}

void PFD::draw()
{
  //glActiveTexture(GL_TEXTURE0);
  //glBindTexture(GL_TEXTURE_2D, texture);
  glUseProgram(shaders_program);
  glBindVertexArray(vao_square);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  return;
}
