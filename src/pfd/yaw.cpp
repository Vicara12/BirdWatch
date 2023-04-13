#include "yaw.h"


YawIndicator::YawIndicator () :
  yaw(0)
{
  compass.setName("Yaw Compass");
  compass.changeShaders("./res/shaders/compass");
  compass.changeTexture("./res/textures/pfd/compass.png", true, GL_CLAMP_TO_EDGE);
  indicator.setName("Yaw Mark");
  indicator.changeTexture("./res/textures/pfd/yaw_mark.png", true, GL_CLAMP_TO_EDGE);
  setTranslation(glm::vec3(0.f));
  setScale(glm::vec3(1.f));
  setRotation(0.f);
}


void YawIndicator::setTranslation (glm::vec3 translation)
{
  compass.setTranslation(glm::vec3(translation[0]+0,
                                   translation[1]+0,
                                   translation[2]+0));
  indicator.setTranslation(glm::vec3(translation[0]+0,
                                     translation[1]+0,
                                     translation[2]+0));
}


void YawIndicator::setScale (glm::vec3 scale)
{
  compass.setScale(glm::vec3(scale[0]*1.0,
                             scale[1]*0.2,
                             scale[2]*1));
  indicator.setScale(glm::vec3(scale[0]*0.07,
                               scale[1]*0.30,
                               scale[2]*1));
}


void YawIndicator::setRotation (float angle)
{
  compass.setRotation(angle);
  indicator.setRotation(angle);
}


bool YawIndicator::init ()
{
  bool all_ok = true;
  if (not compass.init()) {
    std::cout << "Could not init " << compass.name() << "\n";
    all_ok = false;
  } else {
    //texTransLoc = glGetUniformLocation(compass.getShaderProgram(), "texTransf");
    //std::cout << texTransLoc << std::endl;
  }
  if (not indicator.init()) {
    std::cout << "Could not init " << indicator.name() << "\n";
    all_ok = false;
  }
  return all_ok;
}


void YawIndicator::draw ()
{
  //updateTextureTransform();
  compass.draw();
  indicator.draw();
}


std::string YawIndicator::name ()
{
  return "Yaw Indicator";
}


void YawIndicator::setYaw (float yaw)
{
  this->yaw = yaw;
}


void YawIndicator::updateTextureTransform ()
{
  glm::mat4 TexTrans(1.0);
  glUseProgram(compass.getShaderProgram());
  TexTrans = glm::scale(TexTrans, glm::vec3(1, 5, 1));
  glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, &TexTrans[0][0]);
}