#ifndef SERIAL_READER_H_
#define SERIAL_READER_H_

#include <string>
#include <termios.h>

class SerialReader
{
  public:
    // name is just the part that comes after /dev/tty, for example ACM0 or USB0
    SerialReader (std::string name);

    // this constructor will automatically connect to the port if there is only
    // a single tty file available (if not it outputs an error)
    SerialReader ();

    ~SerialReader ();

  private:

    void printError (std::string location);

    bool initiated;
    int serial_port; // file descriptor
};

#endif
