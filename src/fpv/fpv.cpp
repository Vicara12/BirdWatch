#include "fpv.h"
#include <filesystem>
#include <fstream>

FPV::FPV () :
  video_inited(false)
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

bool FPV::preInitVideo ()
{
  if (not video.init(video_source, width, height)) {
    cannotOpenVideoSource();
    return false;
  }
  return video_inited = true;
}

bool FPV::init ()
{
  Drawable::generateSquare(VAO, EBO);
  if (not Drawable::loadProgram("./res/shaders/pfd", shader_program))
    return false;
  if (not video_inited and not preInitVideo())
    return false;
  glUseProgram(shader_program);
  glUniform1i(glGetUniformLocation(shader_program, "texture0"), 0);
  unsigned TGLoc = glGetUniformLocation(shader_program, "TG");
  unsigned preTGLoc = glGetUniformLocation(shader_program, "PreTG");
  // the transform matrices are set to 1 because distortions are performed
  // changing the viewport
  glm::mat4 I(1.f);
  // the -1 on the y axis is because video is flipped on the y axis for some reason
  glm::mat4 scaled = glm::scale(I, glm::vec3(2,-2*float(height)/width,1));
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &scaled[0][0]);
  glUniformMatrix4fv(preTGLoc, 1, GL_FALSE, &I[0][0]);

  return true;
}


void FPV::draw ()
{
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  unsigned char *data = video.loadFrame();
  if (data != NULL)
    loadTexture(data);
  Drawable::draw();
}


void FPV::genTexture ()
{
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // set wrapping and filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void FPV::loadTexture (unsigned char *data)
{
  // load and generate the texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
}



std::vector<Source> FPV::listCameras ()
{
  std::vector<Source> sources;
  std::string camera_path = "/sys/class/video4linux";
  for (const auto &video : std::filesystem::directory_iterator(camera_path)) {
    std::string path = std::string(video.path());
    std::ifstream name_file(path+"/name");
    std::string video_name;
    std::string video_input_file = path.substr(path.find_last_of("/")+1);
    getline(name_file, video_name);
    // each video source has various videoX files, check that the current one
    // is not already included
    bool found = false;
    for (auto source = sources.begin(); source != sources.end(); source++) {
      if (source->first == video_name) {
        found = true;
        source->second.push_back(video_input_file);
        break;
      }
    }
    if (not found) {
      std::vector<std::string> source_file_vec(1, video_input_file);
      sources.push_back(std::make_pair(video_name, source_file_vec));
    }
  }

  return sources;
}


void FPV::cannotOpenVideoSource ()
{
  std::cout << "Available camera sources are:" << std::endl;
  for (const Source &s : listCameras()) {
    std::cout << "   * " << s.first << ":";
    for (int i = s.second.size()-1; i >= 0; i--)
      std::cout << " /dev/" << s.second[i];
    std::cout << std::endl;
  }
}


double FPV::getAR () const
{
  return double(width)/height;
}
