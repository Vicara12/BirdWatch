#include "rectangle.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>


Rectangle::Rectangle () :
  texture_path("./res/textures/pfd/awesomeface.png"),
  shader_path("./res/shaders/pfd"),
  translation(glm::vec3(0.f)),
  scale(glm::vec3(1.f)),
  angle(0.f)
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
  updateTG();
  Drawable::draw();
}


bool Rectangle::init ()
{
  Drawable::generateSquare(VAO, EBO);
  if (not Drawable::loadProgram(shader_path, shader_program) or
      (not Drawable::loadTexture(texture_path, texture, true, false)))
    return false;
  glUniform1i(glGetUniformLocation(shader_program, "texture0"), 0);
  TGLoc = glGetUniformLocation(shader_program, "TG");
  updateTG();
  return true;
}


std::string Rectangle::name ()
{
  return "rectangle";
}


void Rectangle::updateTG ()
{
  glm::mat4 TG(1.f);
  TG = glm::translate(TG, translation);
  TG = glm::rotate(TG, angle, glm::vec3(0.f, 0.f, 1.f));
  TG = glm::scale(TG, scale);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}


void Rectangle::setTranslation (glm::vec3 translation)
{
  this->translation = translation;
}


void Rectangle::setScale (glm::vec3 scale)
{
  this->scale = scale;
}


void Rectangle::setRotation (float angle)
{
  this->angle = angle;
}
