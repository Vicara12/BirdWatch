#include "rectangle.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>


Rectangle::Rectangle () :
  has_alpha(true),
  texture_path("./res/textures/default.png"),
  shader_path("./res/shaders/pfd"),
  translation(glm::vec3(0.f)),
  scale(glm::vec3(1.f)),
  preTG(glm::mat4(1.0)),
  angle(0.f),
  rect_name("Rectangle"),
  texture_border_mode(GL_REPEAT)
{
}


void Rectangle::changeTexture (std::string texture_path, bool has_alpha,
                               unsigned border_mode)
{
  this->texture_path = texture_path;
  this->has_alpha = has_alpha;
  this->texture_border_mode = border_mode;
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
  if (preTGLoc != -1)
    glUniformMatrix4fv(preTGLoc, 1, GL_FALSE, &preTG[0][0]);
  Drawable::draw();
}


bool Rectangle::init ()
{
  Drawable::generateSquare(VAO, EBO);
  if (not Drawable::loadProgram(shader_path, shader_program) or
      (not Drawable::loadTexture(texture_path, texture, has_alpha,
                                 texture_border_mode)))
    return false;
  glUseProgram(shader_program);
  glUniform1i(glGetUniformLocation(shader_program, "texture0"), 0);
  TGLoc = glGetUniformLocation(shader_program, "TG");
  preTGLoc = glGetUniformLocation(shader_program, "PreTG");
  return true;
}


std::string Rectangle::name ()
{
  return rect_name;
}

void Rectangle::setName (std::string name)
{
  this->rect_name = name;
}


void Rectangle::updateTG ()
{
  glm::mat4 TG(1.f);
  TG = glm::translate(TG, translation);
  TG = glm::rotate(TG, angle, glm::vec3(0.f, 0.f, 1.f));
  TG = glm::scale(TG, scale);
  glUseProgram(shader_program);
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


unsigned Rectangle::getShaderProgram ()
{
  return shader_program;
}


void Rectangle::setPreTG (glm::mat4 PreTG)
{
  this->preTG = glm::mat4(PreTG);
}
