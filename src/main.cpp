#include "birdwatcher.h"
#include "datahandler/serial.h"

void echoSerial ()
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
      char buff [2048];
      while (serial.getNAvailableBytes() == 0);
      int n = serial.readPort(buff, 2048);
      for (int i = 0; i < n; i++) std::cout << buff[i];
    }
  }
}

int main ()
{
  BirdWatcher();
  //echoSerial();
  return 0;
}
