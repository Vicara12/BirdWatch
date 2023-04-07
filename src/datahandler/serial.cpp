#include "serial.h"
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>


void SerialReader::printError (std::string location)
{
  printf("SERIAL ERROR: %s \"%s\"\n", location, strerror(errno));
}


SerialReader::SerialReader (std::string name) :
  initiated(false)
{
  serial_port = open(name.c_str(), O_RDWR);

  if (serial_port < 0)

