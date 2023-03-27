#include "attitude.h"
#include <glm/ext/vector_float3.hpp>
#include <string>


AttitudeInd::AttitudeInd () : pitch(0), roll(0)
{
  attitude_ind.setName("Attitude Background");
  attitude_ind.changeShaders("./res/shaders/attitude");
  attitude_ind.changeTexture("./res/textures/pfd/PFD_background.png",
                           true, GL_CLAMP_TO_EDGE);
  setTranslation(glm::vec3(0.f));
  setScale(glm::vec3(1.f));
  setRotation(0.f);
}


void AttitudeInd::draw ()
{
  loadTextureTransformMat();
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, border);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, statics);
  attitude_ind.draw();
}


bool AttitudeInd::init ()
{
  bool all_ok = true;
  if (not attitude_ind.init() or
      not Drawable::loadTexture("./res/textures/pfd/border.png", border,
                                true, GL_CLAMP_TO_EDGE) or
      not Drawable::loadTexture("./res/textures/pfd/statics.png", statics,
                                true, GL_CLAMP_TO_EDGE)) {
    std::cout << "Could not init " << attitude_ind.name() << "\n";
    all_ok = false;
  } else {
    unsigned program = attitude_ind.getShaderProgram();
    glUseProgram(program);
    glUniform1i(glGetUniformLocation(program, "border"), 1);
    glUniform1i(glGetUniformLocation(program, "statics"), 2);
    texTransLoc = glGetUniformLocation(program, "texTransform");
  }
  return all_ok;
}


std::string AttitudeInd::name ()
{
  return "Attitude Indicator";
}


void AttitudeInd::setTranslation (glm::vec3 translation)
{
  attitude_ind.setTranslation(translation);
}


void AttitudeInd::setScale (glm::vec3 scale)
{
  attitude_ind.setScale(glm::vec3(scale[0],
                                scale[1],
                                scale[2]));
}


void AttitudeInd::setRotation (float angle)
{
  attitude_ind.setRotation(angle);
}


void AttitudeInd::setPitchRoll (float pitch, float roll)
{
  this->pitch = pitch;
  this->roll = roll;
}


void AttitudeInd::loadTextureTransformMat ()
{
  pitch = 10;
  roll = 00;
  glm::mat4 TG(1.f);
  TG = glm::translate(TG, glm::vec3(0.5f, 0.5f, 0.f));
  TG = glm::scale(TG, glm::vec3(1.f, 3.f/10.f, 1.f));
  TG = glm::rotate(TG, float(roll*M_PI/180.f), glm::vec3(0.f, 0.f, 1.f));
  TG = glm::translate(TG, glm::vec3(-0.5f, -0.5f+pitch*1.65/90, 0.f));
  glUseProgram(attitude_ind.getShaderProgram());
  glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, &TG[0][0]);
}
