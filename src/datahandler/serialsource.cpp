#include "serialsource.h"


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
    printf("-> %s\n", last_full_line);
  }
  data_available = false;
  return data;
}


void SerialSource::processNewData ()
{
  while (serial.getNAvailableBytes() > 0) {
    char buffer[512];
    int buffer_len = serial.readPort(buffer, sizeof(buffer));
    for (int i = 0; i < buffer_len; i++) {
      if (buffer[i] == '\n') {
        if (line_size_fixed and cline_pos != expected_line_size)
          invalid_line = true;
        if (invalid_line)
          invalid_line = false;
        else
          data_available = true;
        current_line[cline_pos] = 0;
        last_line_size = cline_pos;
        swapBuffers();
      } else if (buffer[i] != '\r') {
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
