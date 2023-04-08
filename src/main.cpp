#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include "pfd/pfd.h"
#include "winhandler/windowhandler.h"
#include "winhandler/textrender.h"
#include "datahandler/serial.h"

void test1 ()
{
  PFD pfd_test;
  pfd_test.setTranslation(glm::vec3(0.5f, 0.f, 0.f));
  pfd_test.setScale(glm::vec3(0.5, 1, 1));
  TextRenderer *text_renderer = TextRenderer::getInstance();
  text_renderer->addText("The quick brown fox jumps over the lazy dog",
                         0, 0.50, glm::vec2(0,0), glm::vec3(1), TXT_CENTER);
  WindowHandler w("BirdWatcher");
  //w.addDrawable(&pfd_test);
  w.addDrawable(text_renderer);
  w.play();
}

void testSerial ()
{
  //SerialReader serial;
  for (auto found : SerialReader::getAvailablePorts("S"))
    std::cout << "Found -> " << found << std::endl;
}

int main ()
{
  testSerial();
  return 0;
}
