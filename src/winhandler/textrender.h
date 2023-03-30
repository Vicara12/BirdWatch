#ifndef TEXTRENDERER_H_
#define TEXTRENDERER_H_


#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "drawable.h"
#include <ft2build.h>
#include FT_FREETYPE_H


enum TextAlignment {TXT_LEFT, TXT_CENTER, TXT_RIGHT};


typedef struct {
  unsigned int textureID;  // ID handle of the glyph texture
  glm::ivec2   size;       // Size of glyph
  glm::ivec2   bearing;    // Offset from baseline to left/top of glyph
  unsigned int advance;    // Offset to advance to next glyph
} Character;


typedef struct {
  std::string text;
  float angle;
  float scale;
  glm::vec2 location;
  glm::vec3 color;
  TextAlignment align_to;
  unsigned width;
  int id;
} TextElement;


class TextRenderer : public Drawable
{
  public:

    static TextRenderer* getInstance ();
    void changeFontPath (const std::string &font_path);
    void changeFontSize (unsigned font_size);
    void changeDefaultWindowSize (unsigned window_width, unsigned window_height);
    bool init ();
    void draw ();
    // returns the text element id or 0 if error
    unsigned long addText (const std::string &text, float angle, float scale,
                  glm::vec2 location, glm::vec3 color, TextAlignment alignment);
    void deleteText (int id);
    void windowReescaled (int height, int width);
    void updateText (int id, const std::string &new_str);

  private:

    TextRenderer ();
    void loadChars ();
    std::string name ();
    void drawTextElement (const TextElement &text);
    void loadVertexArrays ();
    unsigned getTextWidth (const TextElement &text);

    FT_Library ft;
    FT_Face face;
    Character characters[128];
    unsigned VAO, VBO;
    unsigned projLoc, textColLoc;
    std::vector<TextElement> texts;
    bool initiated;
    int next_available_id;
    std::string font_path;
    unsigned font_size;
    unsigned window_width, window_height;

    static TextRenderer *instance;
};

#endif
