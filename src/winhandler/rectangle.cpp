#include "rectangle.h"


Rectangle::Rectangle () :
  texture_path("./res/textures/pfd/awesomeface.png"),
  shader_path("./res/shaders/pfd")
{
}


void Rectangle::changeTexture (std::string texture_path)
{
  this->texture_path = texture_path;
}


void Rectangle::changeShaders (std::string shader_path)
{
  this->shader_path = shader_path;
}


void Rectangle::draw ()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  Drawable::draw();
}


bool Rectangle::init ()
{
  Drawable::generateSquare(VAO, EBO);
  if (not Drawable::loadProgram(shader_path, shader_program) or
      (not Drawable::loadTexture(texture_path, texture, true, false)))
    return false;
  glUniform1i(glGetUniformLocation(shader_program, "texture0"), 0);
  return true;
}
