#include "birdwatcher.h"
#include "datahandler/serialsource.h"
#include "datahandler/filesource.h"
#include "winhandler/textrender.h"


BirdWatcher::BirdWatcher (int argc, char **argv) :
  window_width(1800),
  window_height(900),
  init_ok(false),
  window("Bird Watcher")
{
  std::string config_file_path;
  if (argc != 2) {
    std::cout << "Config file path not provided as argument." << std::endl
              << "Path to the config file to be used: ";
    std::cin >> config_file_path;
  } else
    config_file_path = std::string(argv[1]);

  if (not init(config_file_path))
    return;

  init_ok = true;
  run();
}


BirdWatcher::~BirdWatcher ()
{
  // if the program did not init well, these objects will not exist (removing
  // them will cause sigfault)
  if (not init_ok)
    return;

  for (Drawable *pannel : pannels)
    delete pannel;
  delete data_handler.getDataSource();
  window.deleteDisplay();
}


bool BirdWatcher::init (std::string config_file_path)
{
  if (not config.parseConfigFile(config_file_path))
    return false;

  if (config.hasField("window_size")) {
    int window_size;
    config.getField("window_size", window_size);
    if (window_size < 0) {
      std::cout << "ERROR: config file field window_size must be positive" << std::endl;
    }
    window_width = 2*window_size;
    window_height = window_size;
  }

  TextRenderer *text_renderer = TextRenderer::getInstance();
  text_renderer->changeDefaultWindowSize(window_width, window_height);
  text_renderer->changeFontSize(window_width/3);

  return addPFD() and initDataSource() and initWindowHandler() and initScreenMessages();
}


void BirdWatcher::run ()
{
  while (window.windowOpen()) {
    data_handler.updateData();
    updateScreenMessages();
    window.update();
  }
}


bool BirdWatcher::initScreenMessages ()
{
  TextRenderer *txt = TextRenderer::getInstance();
  eof_txt_id = txt->addText("END OF FILE", 0, 0.5, glm::vec2(0,-0.05),
                            glm::vec3(1,0,0), TXT_CENTER);
  no_data_txt_id = txt->addText("NO DATA", 0, 0.5, glm::vec2(0,-0.05),
                                glm::vec3(1,0,0), TXT_CENTER);
  screen_messages.addMessage(eof_txt_id);
  screen_messages.setMessageStatus(eof_txt_id, false);
  screen_messages.addMessage(no_data_txt_id);
  screen_messages.setMessageStatus(no_data_txt_id, false);
  return true;
}


void BirdWatcher::updateScreenMessages ()
{
  bool reached_eof = not data_handler.thereIsMoreData();
  screen_messages.setMessageStatus(eof_txt_id, reached_eof);
  if (not reached_eof)
    screen_messages.setMessageStatus(no_data_txt_id, not data_handler.checkDataLink());
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
  std::string data_source_type;
  if (not config.getDataSourceField("type", data_source_type)) {
    std::cout << "ERROR: config file does not contain data_source \"type\" field"
              << std::endl;
    return false;
  }

  DataSource *ds = NULL;
  if (data_source_type == "serial") {
    if (not initSerialSource(&ds))
      return false;
  } else if (data_source_type == "file") {
    if (not initFileSource(&ds))
      return false;
  } else {
    std::cout << "ERROR: config file has invalid data_source \"type\" field: "
              << data_source_type << std::endl;
    return false;
  }

  std::string data_format;
  if (not config.getField("data_format", data_format)) {
    std::cout << "ERROR: config file does not contain data_format field" << std::endl;
    return false;
  } else if (data_format == "ascii")
    ds->setDataFormat(DF_ASCII);
  else if (data_format == "binary")
    ds->setDataFormat(DF_BINARY);
  else {
    std::cout << "ERROR: invalid data_format field, supported values are ascii and binary"
              << std::endl;
    return false;
  }

  std::string separator = " ";
  config.getField("data_separator", separator);
  ds->setAsciiDataSeparator(separator[0]);

  data_handler.setDataSource(ds);
  std::vector<std::string> data_fields;
  if (not config.getField("data_fields", data_fields)) {
    std::cout << "ERROR: config file does not contain \"data_fields\"" << std::endl;
    return false;
  }
  data_handler.setDataFields(data_fields);

  bool print_data = false;
  config.getField("print_data", print_data);
  data_handler.printDataInTerminal(print_data);

  int no_data_timeout = 250;
  config.getField("no_data_timeout", no_data_timeout);
  data_handler.setNoDataTimeout(no_data_timeout);

  return true;
}

bool BirdWatcher::initSerialSource (DataSource **ds)
{
  std::string port;
  int baud;

  if (not config.getDataSourceField("port", port)) {
    std::cout << "ERROR: config file does not contain data_source \"port\" field" << std::endl;
    return false;
  }

  if (not config.getDataSourceField("baud", baud)) {
    std::cout << "ERROR: config file does not contain data_source \"baud\" field" << std::endl;
    return false;
  } else {
    // convert from integer with baud to baud definitions (ex: 9600 -> B9600)
    baud = SerialSource::int2Baud(baud);
    if (baud == -1) {
      std::cout << "ERROR: invalid baud rate" << std::endl;
      return false;
    }
  }

  int line_buffer_size = -1;
  config.getDataSourceField("line_buffer_size", line_buffer_size);

  SerialSource *serial_source;
  if (line_buffer_size != -1)
    serial_source = new SerialSource(port, baud, line_buffer_size);
  else
    serial_source = new SerialSource(port, baud);

  if (not serial_source->initOk())
    return false;

  *ds = serial_source;
  return true;
}


bool BirdWatcher::initFileSource (DataSource **ds)
{
  std::string file_path;
  if (not config.getDataSourceField("file_path", file_path)) {
    std::cout << "ERROR: config file does not contain data_source \"file_path\" field"
              << std::endl;
    return false;
  }

  FileSource *file_source = new FileSource(file_path);

  if (not file_source->initOk())
    return false;

  *ds = file_source;
  return true;
}


bool BirdWatcher::initWindowHandler ()
{
  for (Drawable *pannel : pannels)
    window.addDrawable(pannel);
  window.addDrawable(TextRenderer::getInstance());
  window.setRes(window_width, window_height);
  if (not window.initialSetup())
    return false;
  return true;
}
