#include "filesource.h"
#include <sstream>
#include <iostream>


FileSource::FileSource (std::string file_path) :
  init_ok(false),
  reached_eof(false),
  fatal_fail(false),
  first_timestamp(-1)
{
  file = std::ifstream(file_path);
  if (not file.is_open()) {
    std::cout << "ERROR: could not open file " << file_path << std::endl;
    return;
  }
  init_ok = true;
  parseNewLine(0);
}


FileSource::~FileSource ()
{
  file.close();
}


bool FileSource::newDataAvailable ()
{
  return not (reached_eof or fatal_fail) and getMillis() > new_data_timestamp;
}


std::vector<float> FileSource::getLatestData ()
{
  time_last_data_read = new_data_timestamp;
  std::vector<float> new_data = this->new_data;
  parseNewLine(0);
  return new_data;
}


bool FileSource::initOk () const
{
  return init_ok;
}


void FileSource::parseNewLine (int current_recursion)
{
  reached_eof = file.eof();
  // if there has been more than 5 attempts to read a line there is something
  // very wrong with the file, abort
  if (current_recursion > 5)
    fatal_fail = true;
  if (reached_eof or fatal_fail)
    return;
  std::string data;
  std::getline(file, data);
  reached_eof = file.eof();

  if (data_format == DF_BINARY)
    parseBinaryLine(data, current_recursion);
  else
    parseAsciiLine(data, current_recursion);
}


void FileSource::parseBinaryLine (std::string data, int current_recursion)
{
  // as data is sent as binary, there is a given probability that the charater
  // \n is sent as data, instead of as a line jump. if that's detected read more
  // of the line
  while ((line_size_fixed and (int)data.length() < 4*(1+expected_line_size)) or
      data.length() < 4)
  {
    std::string more_data;
    std::getline(file, more_data);
    data += more_data;
  }
  new_data.clear();
  float *float_data_array = (float*)(data.c_str());
  long *this_data_timestamp = (long*)(data.c_str());
  for (int i = 1; i < (int)data.length()/4; i++)
    data.push_back(float_data_array[i]);
  if (line_size_fixed and (int)data.size() != expected_line_size) {
    std::cout << "WARNING: incorrect line size (expected " << expected_line_size
              << " elements but got " << new_data.size() << ") in line:" << std::endl
              << "   " << data << std::endl;
    parseNewLine(current_recursion+1);
    return;
  }
  if (first_timestamp == -1)
    first_timestamp = *this_data_timestamp;
  // the substraction is made so that if data begins at time x the user does not
  // have to wait until the program has been running for x to bein seeing the data
  new_data_timestamp = *this_data_timestamp - first_timestamp;
}


void FileSource::parseAsciiLine (std::string data, int current_recursion)
{
  std::stringstream ss(data);
  std::string element;
  long long this_data_timestamp;
  // if line is empty jump to next one
  if (not std::getline(ss, element, separator)) {
    parseNewLine(current_recursion+1);
    return;
  }
  try {
    std::stringstream time_stamp_str(element);
    time_stamp_str >> this_data_timestamp;
  } catch ( ... ) {
    std::cout << "ERROR: detected invalid timestamp \"" << element << "\"" << std::endl;
  }
  new_data.clear();
  while (std::getline(ss, element, separator)) {
    try {
      new_data.push_back(std::stof(element));
    } catch ( ... ) {
      new_data.push_back(0);
      std::cout << "WARNING: found invalid data value \"" << element << "\"" << std::endl;
    }
  }
  if (line_size_fixed and (int)new_data.size() != expected_line_size) {
    std::cout << "WARNING: incorrect line size (expected " << expected_line_size
              << " elements but got " << new_data.size() << ") in line:" << std::endl
              << "   " << data << std::endl;
    parseNewLine(current_recursion+1);
    return;
  }

  if (first_timestamp == -1)
    first_timestamp = this_data_timestamp;
  // do not set it unless all data has been read to avoid discarding a line
  // but marking it as new data
  new_data_timestamp = this_data_timestamp - first_timestamp;
}


bool FileSource::thereIsMoreData () const
{
  return not reached_eof;
}
