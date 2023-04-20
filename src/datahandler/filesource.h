#ifndef FILESOURCE_H_
#define FILESOURCE_H_

#include "datasource.h"
#include <fstream>

// the first value or 4 bytes (depending on BINARY or ASCII mode)
// of data should be the time when the data sample was taken in milliseconds
class FileSource : public DataSource
{
public:

  FileSource (std::string file_path);
  ~FileSource ();
  bool newDataAvailable ();
  std::vector<float> getLatestData ();
  bool initOk () const;
  bool thereIsMoreData () const;

private:
  void parseNewLine (int current_recursion);
  void parseBinaryLine (std::string data, int current_recursion);
  void parseAsciiLine (std::string data, int current_recursion);

  std::ifstream file;
  bool init_ok;
  bool reached_eof;
  bool fatal_fail;
  long new_data_timestamp;
  long first_timestamp;
  std::vector<float> new_data;
};

#endif
