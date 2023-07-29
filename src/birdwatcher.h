#ifndef BIRDWATCHER_H_
#define BIRDWATCHER_H_

#include "winhandler/drawable.h"
#include "winhandler/windowhandler.h"
#include "datahandler/datahandler.h"
#include "datahandler/config.h"
#include "winhandler/screenmessages.h"
#include "fpv/fpv.h"
#include "pfd/pfd.h"
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
  bool addFPV ();
  bool initDataSource ();
  bool initSerialSource (DataSource **ds);
  bool initFileSource (DataSource **ds);
  bool initWindowHandler ();
  bool initScreenMessages ();
  bool initPannels ();
  void updateScreenMessages ();
  void composePannels ();

  int window_width, window_height;
  int eof_txt_id, no_data_txt_id;
  bool init_ok;
  int video_pannel_id;
  int pfd_pannel_id;
  FPV *fpv;
  PFD *pfd;
  Config config;
  std::vector<Drawable *> pannels;
  DataHandler data_handler;
  WindowHandler window;
  ScreenMessages screen_messages;
};

#endif
