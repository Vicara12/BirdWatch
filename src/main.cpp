#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include "pfd/pfd.h"
#include "winhandler/windowhandler.h"
#include "winhandler/textrender.h"
#include "datahandler/serialsource.h"

#include <unistd.h>
#include <stdlib.h>

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
  SerialSource ss("USB0", B9600);
  //ss.setDataFormat(DF_BINARY);
  //ss.setExpectedLineSize(12);
  ss.setDataFormat(DF_ASCII);
  while(1) {
    if (ss.newDataAvailable()) {
      std::vector<float> new_data = ss.getLatestData();
      for (auto item : new_data)
        std::cout << item << " ";
      std::cout << "\n";
    }
  }
}

int main ()
{
  testSerial();
  return 0;
}
