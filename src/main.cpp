#include <iostream>
#include "pfd/pfd.h"
#include "winhandler/windowhandler.h"

int main ()
{
  WindowHandler w("BirdWatcher");
  w.play();
  return 0;
}
