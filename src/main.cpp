#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include "pfd/pfd.h"
#include "winhandler/windowhandler.h"
#include "winhandler/textrender.h"
#include "datahandler/serial.h"

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
  SerialReader serial;
  std::vector<std::string> ports = SerialReader::getAvailablePorts("USB");
  if (ports.size() == 0)
    std::cout << "ERROR: no suitable serial port found\n";
  else if (ports.size() > 1)
    std::cout << "ERROR: more than one suitable serial port found\n";
  else {
    if (not serial.init(ports[0], B9600))
      return;
    while (1) {
      std::string input;
      while (serial.getNAvailableBytes() == 0) sleep(1);
      std::cout << serial.readPort();
      std::cin >> input;
      serial.writePort(input);
    }
  }
}

int main ()
{
  testSerial();
  return 0;
}
