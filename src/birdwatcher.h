#ifndef BIRDWATCHER_H_
#define BIRDWATCHER_H_

#include "winhandler/drawable.h"
#include "winhandler/windowhandler.h"
#include "datahandler/datahandler.h"
#include "datahandler/config.h"
#include <vector>

class BirdWatcher
{
public:
  BirdWatcher(int argc, char **argv);
  ~BirdWatcher();

private:
  bool init (std::string config_file_path);
  void run ();

  bool addPFD ();
  bool initDataSource ();
  bool initSerialSource (DataSource **ds);
  bool initFileSource (DataSource **ds);
  bool initWindowHandler ();

  int window_width, window_height;
  bool init_ok;
  Config config;
  std::vector<Drawable*> pannels;
  DataHandler data_handler;
  WindowHandler window;
};

#endif
