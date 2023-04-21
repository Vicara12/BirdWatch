#include "birdwatcher.h"
#include "datahandler/serialsource.h"
#include "datahandler/filesource.h"
#include "winhandler/textrender.h"


BirdWatcher::BirdWatcher () :
  window("Bird Watcher")
{
  if (not init())
    return;
  run();
}


BirdWatcher::~BirdWatcher ()
{
  for (Drawable *pannel : pannels)
    delete pannel;
  delete data_handler.getDataSource();
  window.deleteDisplay();
}


bool BirdWatcher::init ()
{
  TextRenderer *text_renderer = TextRenderer::getInstance();
  text_renderer->changeFontSize(200);

  return addPFD() and initDataSource() and initWindowHandler();
}


void BirdWatcher::run ()
{
  TextRenderer *txt = TextRenderer::getInstance();
  bool no_data_drawn = false;
  bool no_more_data_drawn = false;
  unsigned long no_data_txt_id = 0;
  while (window.windowOpen()) {
    data_handler.updateData();

    if (not no_more_data_drawn and not data_handler.thereIsMoreData()) {
      no_data_txt_id = txt->addText("END OF FILE", 0, 0.5, glm::vec2(0,-0.05),
                                    glm::vec3(1,0,0), TXT_CENTER);
      no_data_drawn = true;
    }
    // data connection has been lost, add NO DATA text
    else if (not no_data_drawn and not data_handler.checkDataLink()) {
      no_data_txt_id = txt->addText("NO DATA", 0, 0.5, glm::vec2(0,-0.05),
                                    glm::vec3(1,0,0), TXT_CENTER);
      no_data_drawn = true;
    }
    // if data connection reestablished, delete NO DATA text
    else if (no_data_drawn and data_handler.checkDataLink()) {
      txt->deleteText(no_data_txt_id);
      no_data_drawn = false;
    }

    window.update();
  }
}


bool BirdWatcher::addPFD ()
{
  PFD *pfd = new PFD;
  pfd->setTranslation(glm::vec3(0.5f, 0.f, 0.f));
  pfd->setScale(glm::vec3(0.5, 1, 1));
  pannels.push_back(pfd);
  data_handler.setPFD(pfd);
  return true;
}


bool BirdWatcher::initDataSource ()
{
  bool use_serial_source = true;
  DataSource *ds;
  if (use_serial_source) {
    SerialSource *serial_source = new SerialSource("USB0", B9600);
    if (not serial_source->initOk())
      return false;
    serial_source->setDataFormat(DF_ASCII);
    serial_source->setFieldsPerLine(3);
    ds = serial_source;
  } else {
    FileSource *file_source = new FileSource("./test/out_ascii.txt");
    if (not file_source->initOk())
      return false;
    file_source->setDataFormat(DF_ASCII);
    file_source->setFieldsPerLine(3);
    ds = file_source;
  }

  data_handler.setDataSource(ds);
  std::vector<std::string> data_fields;
  data_fields.push_back(std::string("YPR"));
  data_handler.setDataFields(data_fields);
  data_handler.printDataInTerminal(true);
  return true;
}


bool BirdWatcher::initWindowHandler ()
{
  for (Drawable *pannel : pannels)
    window.addDrawable(pannel);
  window.addDrawable(TextRenderer::getInstance());
  window.initialSetup();
  return true;
}
