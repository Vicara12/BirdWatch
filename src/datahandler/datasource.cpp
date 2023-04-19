#include "datasource.h"
#include <sys/time.h>

DataSource::DataSource () :
  data_format(DF_ASCII),
  line_size_fixed(false),
  separator(' '),
  time_last_data_read(0)
{
}


void DataSource::setDataFormat (DataFormat data_format)
{
  this->data_format = data_format;
}


void DataSource::setFieldsPerLine (int expected_line_size)
{
  this->expected_line_size = expected_line_size;
  line_size_fixed = true;
}


void DataSource::setAsciiDataSeparator (char separator)
{
  if (data_format == DF_BINARY)
    printf("WARNING: setting separator with binary data does not make sense\n");
  this->separator = separator;
}


long long DataSource::getTimeLastData () const
{
  return getMillis() - time_last_data_read;
}


long long DataSource::getMillis () const
{
  struct timeval tv;
  gettimeofday(&tv,NULL);
  return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}
