#include "drawable.h"
#include <fstream>
#include "stb_image/stb_image.h"

Drawable::Drawable ()
{
}


bool Drawable::loadShader (GLenum shader_type, const std::string &path, unsigned &shader)
{
  std::ifstream shader_file(path);

  if (not shader_file.is_open()) {
    std::cout << "ERROR: could not open shader file '" << path << "'.\n";
    return false;
  }
  std::string shader_content = "";
  while (not shader_file.eof()) {
    std::string line;
    getline(shader_file, line);
    shader_content.append(line+"\n");;
  }
  const char *source_c_str = shader_content.c_str();
  shader = glCreateShader(shader_type);
  glShaderSource(shader, 1, &source_c_str, NULL);
  glCompileShader(shader);

  // check compilation was successful
  int success;
  char compile_result[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (not success) {
    glGetShaderInfoLog(shader, 512, NULL, compile_result);
    std::cout << "ERROR: could not compile shader '" << path << "', error(s):\n";
    std::cout << "\n" << compile_result << std::endl;
    return false;
  }

  return true;
}


bool Drawable::loadProgram (std::string path, unsigned &program)
{
  unsigned vs, fs;
  if (not Drawable::loadShader(GL_VERTEX_SHADER, path+".vs", vs) or
        not Drawable::loadShader(GL_FRAGMENT_SHADER, path+".fs", fs))
      return false;
  program = glCreateProgram();
  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);

  // check linking errors
  int success;
  char error_info [512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (not success) {
    glGetProgramInfoLog(program, 512, NULL, error_info);
    std::cout << "ERROR: error when linking shaders:\n\n";
    std::cout << error_info << std::endl;
    return false;
  }

  glDeleteShader(vs);
  glDeleteShader(fs);
  return true;
}


bool Drawable::loadTexture (std::string path, unsigned &texture, bool has_alpha, bool flip)
{
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // set wrapping and filtering options
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // load and generate the texture
  int width, height, nr_channels;
  stbi_set_flip_vertically_on_load(flip);
  unsigned char *image_data = stbi_load(path.c_str(), &width,
  &height, &nr_channels, 0);

  if (image_data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                 (has_alpha ? GL_RGBA : GL_RGB), GL_UNSIGNED_BYTE, image_data);
    glGenerateMipmap(GL_TEXTURE_2D); // this is to avoid re-calling the previous
                                     // command with each mipmap level
  } else {
    std::cout << "ERROR: can't find texture '" << path << "'.\n";
    return false;
  }

  stbi_image_free(image_data);
  return true;
}



void Drawable::generateSquare (unsigned &VAO)
{
  //                    positon
  float vertices [] = { 0.5f, 0.5f, 0.0f,
                        0.5f,-0.5f, 0.0f,
                       -0.5f,-0.5f, 0.0f,
                       -0.5f, 0.5f, 0.0f};
  unsigned indices [] = { 0, 1, 3,
                          1, 2, 3  };

  unsigned EBO, VBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  // position
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glBindVertexArray(VAO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}
