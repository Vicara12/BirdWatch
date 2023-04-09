#ifndef SERIAL_SOURCE_H_
#define SERIAL_SOURCE_H_

#include "datasource.h"
#include "serial.h"

class SerialSource : public DataSource
{
  public:
    // item size is the size of float in bytes
    SerialSource (std::string port, int baud, int line_buffer_size = 2048);
    bool serialInitiated ();
    bool newDataAvailable ();
    std::vector<float> getLatestData ();
    ~SerialSource();

  private:
    void processNewData ();
    void swapBuffers ();

    bool serial_init_ok;
    bool data_available;
    bool invalid_line;
    SerialReader serial;
    char *last_full_line, *current_line;
    int cline_pos, last_line_size;
    int line_buffer_size;
};


#endif
