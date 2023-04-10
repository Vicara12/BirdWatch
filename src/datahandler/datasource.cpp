#include "datasource.h"

DataSource::DataSource () :
  data_format(DF_ASCII),
  line_size_fixed(false),
  separator(' ')
{
}


void DataSource::setDataFormat (DataFormat data_format)
{
  this->data_format = data_format;
}


void DataSource::setExpectedLineSize(int expected_line_size)
{
  if (data_format == DF_ASCII)
    printf("WARNING: setting expected line size with ASCII data is unsuported\n");
  this->expected_line_size = expected_line_size;
  line_size_fixed = true;
}


void DataSource::setAsciiDataSeparator (char separator)
{
  if (data_format == DF_BINARY)
    printf("WARNING: setting separator with binary data does not make sense\n");
  this->separator = separator;
}
