#ifndef BIRDWATCHER_H_
#define BIRDWATCHER_H_

#include "winhandler/drawable.h"
#include "winhandler/windowhandler.h"
#include "datahandler/datahandler.h"
#include <vector>

class BirdWatcher
{
public:
  BirdWatcher();
  ~BirdWatcher();

private:
  bool init ();
  void run ();

  bool addPFD ();
  bool initDataSource ();
  bool initWindowHandler ();

  std::vector<Drawable*> pannels;
  DataHandler data_handler;
  WindowHandler window;
};

#endif
