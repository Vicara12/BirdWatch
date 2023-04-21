#include "attitude.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <string>


AttitudeInd::AttitudeInd () : pitch(0), roll(0)
{
  attitude_ind.setName("Attitude Background");
  attitude_ind.changeShaders("./res/shaders/attitude");
  attitude_ind.changeTexture("./res/textures/pfd/PFD_background.png",
                           true, GL_CLAMP_TO_EDGE);
  addPaneElement(PaneElement(&attitude_ind));

  angle_ind.setName("Angle Indicator");
  angle_ind.changeTexture("./res/textures/pfd/roll_ind.png",
                           true, GL_CLAMP_TO_EDGE);
  addPaneElement(PaneElement(&angle_ind));
}


void AttitudeInd::draw ()
{
  loadTextureTransformMat();
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, border);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, statics);
  attitude_ind.draw();
  angleIndicatorLoadPreTG();
  angle_ind.draw();
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
  if (not angle_ind.init()) {
    std::cout << "Could not init " << angle_ind.name() << "\n";
    all_ok = false;
  }
  return all_ok;
}


std::string AttitudeInd::name ()
{
  return "Attitude Indicator";
}


void AttitudeInd::setPitchRoll (float pitch, float roll)
{
  while (pitch > 180) pitch -= 360;
  while (pitch <= -180) pitch += 360;
  if (pitch > 90)       pitch = 180 - pitch;
  else if (pitch < -90) pitch = -180 - pitch;
  this->pitch = pitch;
  this->roll = roll;
}


void AttitudeInd::loadTextureTransformMat ()
{
  float correction = pitch*0.49/90;
  glm::mat4 TG(1.f);
  TG = glm::translate(TG, glm::vec3(0.5f, 0.5f+correction, 0.f));
  TG = glm::scale(TG, glm::vec3(1.f, 3.f/10.f, 1.f));
  TG = glm::rotate(TG, float(roll*M_PI/180.f), glm::vec3(0.f, 0.f, 1.f));
  TG = glm::translate(TG, glm::vec3(-0.5f, -0.5f, 0.f));
  glUseProgram(attitude_ind.getShaderProgram());
  glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, &TG[0][0]);
}


void AttitudeInd::angleIndicatorLoadPreTG()
{
  // limit roll inidicated by the upper triangle to [-45, 45]
  float bank_max = 50;
  float roll_indicated = (roll > bank_max ? bank_max : roll);
  roll_indicated = (roll_indicated < -bank_max ? -bank_max : roll_indicated);
  glm::mat4 preTG(1.f);
  preTG = glm::rotate(preTG, float(-roll_indicated*M_PI/180.f), glm::vec3(0, 0, 1));
  preTG = glm::translate(preTG, glm::vec3(0.f, 0.432f, 0.f));
  preTG = glm::scale(preTG, glm::vec3(0.1));
  angle_ind.setPreTG(preTG);
}
