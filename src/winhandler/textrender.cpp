#include "textrender.h"
#include <GL/glew.h>


TextRenderer *TextRenderer::instance = new TextRenderer();


std::string TextRenderer::name () {return "Text Renderer";}


TextRenderer::TextRenderer () :
  initiated(false),
  next_available_id(-1),
  font_path("./res/fonts/Bebas-Regular.ttf"),
  font_size(75),
  window_width(800),
  window_height(600)
{
}


TextRenderer* TextRenderer::getInstance ()
{
  return instance;
}


bool TextRenderer::init ()
{
  if (initiated)
    return true;

  if (FT_Init_FreeType(&ft)) {
    std::cout << "ERROR: could not init FreeType library\n";
    return false;
  }

  if (FT_New_Face(ft, font_path.c_str(), 0, &face)) {
    std::cout << "ERROR: could not load font " << font_path << "\n";
    return false;
  }

  loadChars();
  loadVertexArrays();
  FT_Done_Face(face);
  FT_Done_FreeType(ft);

  if (not Drawable::loadProgram("./res/shaders/textrender", shader_program)) {
    std::cout << "ERROR: could not load shader for font rendering\n";
    return false;
  }

  projLoc = glGetUniformLocation(shader_program, "projection");
  textColLoc = glGetUniformLocation(shader_program, "textColor");
  windowReescaled(window_height, window_width);

  // widths could not be set at addText because the font was not loaded yet and
  // hence the widths were unknown
  for (unsigned long i = 0; i < texts.size(); i++)
    texts[i].width = getTextWidth(texts[i]);

  initiated = true;
  return true;
}


void TextRenderer::loadVertexArrays ()
{
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glBindVertexArray(VAO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}


void TextRenderer::loadChars () {
  FT_Set_Pixel_Sizes(face, 0, font_size);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

  for (unsigned char c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      std::cout << "ERROR: failed to load character " << (char(c)) << "\n";
      continue;
    }
    unsigned width = face->glyph->bitmap.width;
    unsigned rows  = face->glyph->bitmap.rows;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, rows, 0, GL_RED,
                 GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    characters[c].textureID = texture,
    characters[c].size      = glm::ivec2(width, rows),
    characters[c].bearing   = glm::ivec2(face->glyph->bitmap_left,
                                         face->glyph->bitmap_top),
    characters[c].advance   = face->glyph->advance.x;
  }
}


void TextRenderer::draw ()
{
  if (not initiated) {
    std::cout << "ERROR: cannot draw text if TextRenderer init has not been called\n";
    return;
  }
  for (TextElement text : texts)
    drawTextElement(text);
}


unsigned long TextRenderer::addText (const std::string &text, float angle, float scale,
                            glm::vec2 location, glm::vec3 color,
                            TextAlignment alignment)
{
  TextElement te;
  te.text = text;
  te.angle = angle;
  te.scale = scale;
  te.location = location;
  te.color = color;
  te.align_to = alignment;
  te.id = next_available_id;
  // if not initiated this will be done in the init function
  if (initiated)
    te.width = getTextWidth(te);
  next_available_id++;
  texts.push_back(te);
  return te.id;
}


void TextRenderer::drawTextElement (const TextElement &text)
{
  // activate corresponding render state
  glUseProgram(shader_program);
  glUniform3fv(glGetUniformLocation(shader_program, "textColor"), 1, &text.color[0]);
  glActiveTexture(GL_TEXTURE0);
  glBindVertexArray(VAO);

  float x = text.location.x;
  float y = text.location.y;

  // scale from range +-1 to +- 0.5*width/height
  x = x*window_width/2;
  y = y*window_height/2;

  if (text.align_to == TXT_CENTER)
    x -= text.width/2.0;
  else if (text.align_to == TXT_RIGHT)
    x -= text.width;

  for (char c : text.text)  {
    Character ch = characters[(int)c];

    float xpos = x + ch.bearing.x * text.scale;
    float ypos = y - (ch.size.y - ch.bearing.y) * text.scale;

    float w = ch.size.x * text.scale;
    float h = ch.size.y * text.scale;

    float vertices[6][4] = {
        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos,     ypos,       0.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 1.0f },

        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos + w, ypos,       1.0f, 1.0f },
        { xpos + w, ypos + h,   1.0f, 0.0f }
      };

    glBindTexture(GL_TEXTURE_2D, ch.textureID);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    x += (ch.advance >> 6) * text.scale; // bitshift by 6 to get value in pixels (2^6 = 64)
  }
}



void TextRenderer::deleteText (int id)
{
  std::vector<TextElement>::iterator it = texts.begin();
  bool found = false;
  while (not found and it != texts.end()) {
    if (it->id == id) {
      found = true;
      texts.erase(it);
    } else {
      it++;
    }
  }
}


void TextRenderer::windowReescaled (int height, int width)
{
  window_height = height;
  window_width = width;
  glUseProgram(shader_program);
  glm::mat4 projection = glm::ortho(-float(width)/2, float(width)/2,
                                    -float(height)/2, float(height)/2);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);
}


unsigned TextRenderer::getTextWidth (const TextElement &text)
{
  unsigned width = 0;
  for (char c : text.text)
    width += (characters[(int)c].advance >> 6) * text.scale;
  return width;
}


void TextRenderer::updateText (int id, const std::string &new_str)
{
  std::vector<TextElement>::iterator it = texts.begin();
  bool found = false;
  while (not found and it != texts.end()) {
    if (it->id == id) {
      found = true;
      it->text = new_str;
      it->width = getTextWidth(*it);
    } else {
      it++;
    }
  }
}


void TextRenderer::changeFontPath (const std::string &font_path)
{
  this->font_path = font_path;
}


void TextRenderer::changeFontSize (unsigned font_size)
{
  this->font_size = font_size;
}


void TextRenderer::changeDefaultWindowSize (unsigned window_width, unsigned window_height)
{
  this->window_width = window_width;
  this->window_height = window_height;
}


void TextRenderer::setTranslation (glm::vec3) {}
void TextRenderer::setScale (glm::vec3) {}
