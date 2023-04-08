#ifndef SERIAL_READER_H_
#define SERIAL_READER_H_

#include <string>
#include <termios.h>
#include <vector>

// this class is written based on the documentation found in this web page
// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/
// (but the bugs are courtesy of my own)

class SerialReader
{
  public:
    SerialReader ();

    // port name is the one that appears after /dev/tty, for example USB0
    // default config should be enough to read data from Arduino
    bool init (std::string port_name, int baud_rate = B9600);
    // returns a vector with all available port names (USB0, ACM0, PS0...)
    static std::vector<std::string> getAvailablePorts (std::string filter = "");
    // write data to the port
    void writePort (std::string message);
    // read data from the port, returns number of read bytes or -1 if error
    int readPort (char *buffer, unsigned buff_size);
    int getNAvailableBytes () const;
    void closePort ();
    // change c_cflags attribute, options are
    //    PARENB: change partity bit (clear by default)
    //    CSTOPB: use two (set) or one (clear) stop bit (clear by default)
    //    CRTSCTS: enable or disable hardware flow control (disabled by default)
    bool changeControlMode (int flag, bool set);
    // change the number of data bits per byte (default is 8), options are
    //    CS5, CS6, CS7, CS8
    bool setBitsPerByte (int n_bytes);
    // change c_lflag attribute, options are
    //    ICANON: enable or disable canonical more (read until \n) (disabled by default)
    //    ECHO, ECHOE and ECHONL: enable echo (disabled by default)
    //    ISIG: enable interpretation of signal characters (disabled by default)
    bool setLocalModes (int flag, bool set);
    // if VMIN is 0, VTIME specifies a time-out from the start of the read call,
    // if VMIN > 0 then VTIME specifies the timeout from the start of the first
    // received character (CAN BE BLOCKING), whereas VMIN always sets the minimum
    // number of characters read before quit (unless there is a timeout)
    bool setVMINandVTIME (int flag, unsigned char value);
    // change baud rate, values are
    // B0, B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800, B2400,
    // B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800
    // (default is B9600)
    bool setBaud (int baud_rate);

    ~SerialReader ();

  private:

    void printError (std::string location);
    bool getTTYConfig (struct termios &tty);
    bool setTTYConfig (const struct termios &tty);

    bool initialized;
    int serial_port; // file descriptor
    struct termios original_tty;
};

#endif
