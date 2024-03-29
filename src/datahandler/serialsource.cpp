#include "serialsource.h"
#include <sstream>
#include <iostream>


SerialSource::SerialSource (std::string port, int baud, int line_buffer_size) :
  data_available(false),
  invalid_line(false),
  cline_pos(0)
{
  if (not serial.init(port, baud)) {
    serial_init_ok = false;
    return;
  } else {
    serial_init_ok = true;
  }
  last_full_line = new char[line_buffer_size];
  current_line = new char[line_buffer_size];
  this->line_buffer_size = line_buffer_size;
}


bool SerialSource::newDataAvailable ()
{
  processNewData();
  return data_available;
}


bool SerialSource::initOk () const
{
  return serial_init_ok;
}


std::vector<float> SerialSource::getLatestData ()
{
  std::vector<float> data;
  processNewData();

  if (data_format == DF_BINARY) {
    // even in read as a char array, data is transmitted as binary, so interpret
    // it as binary floats
    float *read_data_in_float = (float*)last_full_line;
    for (int i = 0; i < last_line_size/4; i++)
      data.push_back(read_data_in_float[i]);
  } else {
    std::stringstream ss(last_full_line);
    std::string element;
    while (std::getline(ss, element, separator))
      try {
        data.push_back(std::stof(element));
      } catch ( ... ) {
        data.push_back(0);
        std::cout << "WARNING: non numerical field found in line:" << std::endl;
        std::cout << "    " << std::string(last_full_line) << std::endl;
      }
  }
  data_available = false;
  return data;
}


void SerialSource::processNewData ()
{
  char buffer[512];
  int buffer_len;
  while ((buffer_len = serial.readPort(buffer, sizeof(buffer))) > 0) {
    for (int i = 0; i < buffer_len; i++) {
      if (buffer[i] == '\n' and cline_pos > 0 and current_line[cline_pos-1] == '\r') {
        // expected_line_size+1 because carriage return is not taken into account
        // if data_format is ascii line length in bytes is undefined
        if (data_format == DF_ASCII or !line_size_fixed or cline_pos == expected_line_size*4+1) {
          data_available = true;
          time_last_data_read = getMillis();
        }
        if (data_available)
        current_line[cline_pos-1] = 0; // -1 to overwrite carriage return
        last_line_size = cline_pos-1;
        swapBuffers();
      } else {
        current_line[cline_pos] = buffer[i];
        cline_pos++;
      }
      if (cline_pos > line_buffer_size-1) {
        invalid_line = true; // this line will be incomplete, so ignore it
        cline_pos = 0;
        printf("ERROR: serial line buffer size is too small\n");
      }
    }
  }
}


void SerialSource::swapBuffers ()
{
  char *pivot = last_full_line;
  last_full_line = current_line;
  current_line = pivot;
  cline_pos = 0;
}


SerialSource::~SerialSource ()
{
  if (serial_init_ok) {
    delete[] last_full_line;
    delete[] current_line;
  }
}


int SerialSource::int2Baud (int baud)
{
  switch (baud) {
    case 50: return B50;
    case 75: return B75;
    case 110: return B110;
    case 134: return B134;
    case 150: return B150;
    case 200: return B200;
    case 300: return B300;
    case 600: return B600;
    case 1200: return B1200;
    case 1800: return B1800;
    case 2400: return B2400;
    case 4800: return B4800;
    case 9600: return B9600;
    case 19200: return B19200;
    case 38400: return B38400;
    case 57600: return B57600;
    case 115200: return B115200;
    case 230400: return B230400;
    case 460800: return B460800;
    default: return -1;
  }
}
