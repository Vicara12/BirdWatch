#include "yaw.h"


YawIndicator::YawIndicator () :
  yaw(0)
{
  compass.setName("Yaw Compass");
  compass.changeShaders("./res/shaders/compass");
  compass.changeTexture("./res/textures/pfd/compass.png", true, GL_CLAMP_TO_EDGE);
  addPaneElement(PaneElement(&compass, glm::vec3(1,0.2,1), glm::vec3(0)));

  indicator.setName("Yaw Mark");
  indicator.changeTexture("./res/textures/pfd/yaw_mark.png", true, GL_CLAMP_TO_EDGE);
  addPaneElement(PaneElement(&indicator, glm::vec3(0.07,0.243,1), glm::vec3(0,0.032,0)));
}




bool YawIndicator::init ()
{
  if (CompositeElement::init()) {
    texTransLoc = glGetUniformLocation(compass.getShaderProgram(), "texTransf");
    return true;
  } else
    return false;
}


void YawIndicator::draw ()
{
  updateTextureTransform();
  CompositeElement::draw();
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
  glm::mat4 TexTrans(1.f);
  TexTrans = glm::translate(TexTrans, glm::vec3(0.5,0.5,0));
  TexTrans = glm::rotate(TexTrans, float(yaw*M_PI/180.f), glm::vec3(0,0,-1));
  TexTrans = glm::scale(TexTrans, glm::vec3(1, 0.2, 1));
  TexTrans = glm::translate(TexTrans, glm::vec3(-0.5,1.5,0));
  glUseProgram(compass.getShaderProgram());
  glUniformMatrix4fv(texTransLoc, 1, GL_FALSE, &TexTrans[0][0]);
}
