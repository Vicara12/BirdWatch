#include "fpv.h"

FPV::FPV ()
{}

void FPV::setScale (glm::vec3)
{
  //
}

void FPV::setTranslation (glm::vec3)
{
  //
}

void FPV::setVideoSource (std::string source)
{
  this->video_source = source;
}

std::string FPV::name ()
{
  return "FPV stream";
}

bool FPV::init ()
{
  Drawable::generateSquare(VAO, EBO);
  if (not Drawable::loadProgram("./res/shaders/pfd", shader_program))
    return false;
  unsigned char *data;
  int width, height;
  if (not video.init("/home/anon/others/videos/yo.mp4", width, height))
    return false;
  if (not video.loadFrame(&data))
    return false;
  glUseProgram(shader_program);
  loadTexture(width, height, data);
  glUniform1i(glGetUniformLocation(shader_program, "texture0"), 0);
  unsigned TGLoc = glGetUniformLocation(shader_program, "TG");
  unsigned preTGLoc = glGetUniformLocation(shader_program, "PreTG");
  // the transform matrices are set to 1 because distortions are performed
  // changing the viewport
  glm::mat4 I(1.f);
  // the -1 on the y axis is because video is flipped on the y axis for some reason
  glm::mat4 scaled = glm::scale(I, glm::vec3(1,-float(height)/width,1));
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &scaled[0][0]);
  glUniformMatrix4fv(preTGLoc, 1, GL_FALSE, &I[0][0]);

  return true;
}


void FPV::draw ()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  Drawable::draw();
}


void FPV::loadTexture (int width, int height, unsigned char *data)
{
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // set wrapping and filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load and generate the texture

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
}
