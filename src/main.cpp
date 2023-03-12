#include <iostream>
#include "pfd/pfd.h"
#include "winhandler/windowhandler.h"

int main ()
{
  PFD pfd_test;
  WindowHandler w("BirdWatcher");
  w.addDrawable(&pfd_test);
  w.play();
  return 0;
}
