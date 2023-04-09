#include "serial.h"
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <filesystem>
#include <algorithm>


void SerialReader::printError (std::string location)
{
  printf("SERIAL ERROR: %s \"%s\"\n", location.c_str(), strerror(errno));
}


SerialReader::SerialReader () :
  initialized(false)
{
}


bool SerialReader::init (std::string port_name, int baud_rate)
{
  port_name = "/dev/tty" + port_name;
  serial_port = open(port_name.c_str(), O_RDWR);
  initialized = false;

  if (serial_port < 0) {
    printError("when opening port");
    return false;
  }
  if(flock(serial_port, LOCK_EX | LOCK_NB) == -1)
    printf("SERIAL WARNING: could not lock %s, it's already locked by other process.",
            port_name.c_str());

  if (not getTTYConfig(original_tty))
    return false;
  struct termios tty = original_tty;

  tty.c_cflag &= ~PARENB; // disable parity bit
  tty.c_cflag &= ~CSTOPB; // only one stop bit used
  tty.c_cflag &= ~CSIZE;
  tty.c_cflag |= CS8;     // 8 data bits per byte
  tty.c_cflag &= ~CRTSCTS; // disable hardware flow control
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
  tty.c_lflag &= ~ICANON; // disable canonical mode
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE;
  tty.c_lflag &= ~ECHONL;
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable special handling of bits
  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  tty.c_cc[VTIME] = 0; // read any quantity of data available without waiting
  tty.c_cc[VMIN] = 0;
  cfsetispeed(&tty, baud_rate);
  cfsetospeed(&tty, baud_rate);

  if (not setTTYConfig(tty))
    return false;

  initialized = true;
  return true;
}


std::vector<std::string> SerialReader::getAvailablePorts (std::string filter)
{
  std::vector<std::string> found;
  for (const auto &entry : std::filesystem::directory_iterator("/dev/")) {
    std::string file_name = entry.path();
    if (file_name.find("/dev/tty") != std::string::npos and
        file_name.find(filter) != std::string::npos)
      found.push_back(file_name.erase(0, 8));
  }
  return found;
}


void SerialReader::writePort (std::string message)
{
  if (not initialized)
    printf("SERIAL ERROR: trying to write to an uninitialized serial port\n");
  else
    write(serial_port, message.c_str(), message.size());
}


int SerialReader::readPort (char *buffer, unsigned buff_size)
{
  if (not initialized) {
    printf("SERIAL ERROR: trying to read from an uninitialized serial port\n");
    return -1;
  } else {
    int x = read(serial_port, buffer, buff_size-1);
    buffer[x] = 0; // end of line
    return x;
  }
}


std::string SerialReader::readPort ()
{
  if (not initialized) {
    printf("SERIAL ERROR: trying to write to an uninitialized serial port\n");
    return "";
  } else {
    char buffer[512];
    int nbytes = 0;
    std::string str_data = "";
    while ((nbytes = read(serial_port, &buffer, sizeof(buffer)-1)) > 0) {
      buffer[nbytes] = 0;
      str_data += std::string(buffer);
    }
  return str_data;
  }
}


int SerialReader::getNAvailableBytes () const
{
  int n_bytes;
  ioctl(serial_port, FIONREAD, &n_bytes);
  return n_bytes;
}


bool SerialReader::changeControlMode (int flag, bool set)
{
  if (not initialized) {
    printf("SERIAL ERROR: trying to configure an uninitialized serial port\n");
    return false;
  }

  struct termios tty;
  if (not getTTYConfig(tty))
    return false;

  if (set) tty.c_cflag |= flag;
  else     tty.c_cflag &= ~flag;

  if (not setTTYConfig(tty))
    return false;
  return true;
}


bool SerialReader::setBitsPerByte (int n_bytes)
{
  if (not initialized) {
    printf("SERIAL ERROR: trying to configure an uninitialized serial port\n");
    return false;
  }

  struct termios tty;
  if (not getTTYConfig(tty))
    return false;

  tty.c_cflag &= ~CSIZE;
  switch (n_bytes) {
    case 5:
      tty.c_cflag |= CS5;
      break;
    case 6:
      tty.c_cflag |= CS6;
      break;
    case 7:
      tty.c_cflag |= CS7;
      break;
    case 8:
      tty.c_cflag |= CS8;
      break;
    default:
      printf("SERIAL ERROR: unsuported number of bits per byte\n");
      return false;
  }

  if (not setTTYConfig(tty))
    return false;
  return true;
}


bool SerialReader::setLocalModes (int flag, bool set)
{
  if (not initialized) {
    printf("SERIAL ERROR: trying to configure an uninitialized serial port\n");
    return false;
  }

  struct termios tty;
  if (not getTTYConfig(tty))
    return false;

  if (set) tty.c_cflag |= flag;
  else     tty.c_cflag &= ~flag;

  if (not setTTYConfig(tty))
    return false;
  return true;
}


bool SerialReader::setVMINandVTIME (int flag, unsigned char value)
{
  if (not initialized) {
    printf("SERIAL ERROR: trying to configure an uninitialized serial port\n");
    return false;
  }

  struct termios tty;
  if (not getTTYConfig(tty))
    return false;

  tty.c_cc[flag] = value;

  if (not setTTYConfig(tty))
    return false;
  return true;
}


bool SerialReader::setBaud (int baud_rate)
{
  if (not initialized) {
    printf("SERIAL ERROR: trying to configure an uninitialized serial port\n");
    return false;
  }

  struct termios tty;
  if (not getTTYConfig(tty))
    return false;

  cfsetispeed(&tty, baud_rate);
  cfsetospeed(&tty, baud_rate);

  if (not setTTYConfig(tty))
    return false;
  return true;
}


void SerialReader::closePort ()
{
  setTTYConfig(original_tty);
  close(serial_port);
  initialized = false;
}


SerialReader::~SerialReader ()
{
  closePort();
}


bool SerialReader::getTTYConfig (struct termios &tty)
{
  if (tcgetattr(serial_port, &tty) != 0) {
    printError("when getting TTY config");
    return false;
  }
  return true;
}


bool SerialReader::setTTYConfig (const struct termios &tty)
{
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
    printError("when writing TTY config");
    return false;
  }
  return true;
}
