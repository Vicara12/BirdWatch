#include <glm/ext/vector_float2.hpp>
#include <glm/ext/vector_float3.hpp>
#include <iostream>
#include "pfd/pfd.h"
#include "winhandler/windowhandler.h"
#include "datahandler/serialsource.h"
#include "datahandler/datahandler.h"
#include "winhandler/textrender.h"

#include <unistd.h>
#include <stdlib.h>

void test1 ()
{
  PFD pfd_test;
  pfd_test.setTranslation(glm::vec3(0.5f, 0.f, 0.f));
  pfd_test.setScale(glm::vec3(0.5, 1, 1));

  SerialSource serial_source("USB0", B9600);
  /*
  if (not serial_source.serialInitiated())
    return;
    */
  serial_source.setDataFormat(DF_ASCII);
  serial_source.setFieldsPerLine(3);

  DataHandler data_handler;
  data_handler.setDataSource(&serial_source);
  data_handler.setPFD(&pfd_test);
  std::vector<std::string> data_fields;
  data_fields.push_back(std::string("YPR"));
  data_handler.setDataFields(data_fields);
  //data_handler.printDataInTerminal(true);

  TextRenderer *text_renderer = TextRenderer::getInstance();
  text_renderer->changeFontSize(200);

  WindowHandler w("BirdWatcher");
  w.addDrawable(&pfd_test);
  w.addDrawable(text_renderer);

  bool no_data_drawn = false;
  unsigned long no_data_txt_id = 0;
  w.initialSetup();
  while (w.windowOpen()) {
    //data_handler.updateData();

    // data connection has been lost, add NO DATA text
    if (not no_data_drawn and not data_handler.checkDataLink()) {
      no_data_txt_id = text_renderer->addText("NO DATA", 0, 0.5, glm::vec2(0,-0.05),
                                              glm::vec3(1,0,0), TXT_CENTER);
      no_data_drawn = true;
      std::cout << "Drawin" << std::endl;
    }
    // if data connection reestablished, delete NO DATA text
    else if (no_data_drawn and data_handler.checkDataLink()) {
      text_renderer->deleteText(no_data_txt_id);
      no_data_drawn = false;
      std::cout << "Erasin" << std::endl;
    }

    w.update();
  }
  w.deleteDisplay();
}

void testSerial ()
{
  SerialSource ss("USB0", B9600);
  if (not ss.serialInitiated())
    return;
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
  //testSerial();
  test1();
  return 0;
}
