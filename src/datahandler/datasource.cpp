#include "datasource.h"

DataSource::DataSource () :
  data_format(DF_ASCII),
  line_size_fixed(false)
{
}


void DataSource::setDataFormat (DataFormat data_format)
{
  this->data_format = data_format;
}


void DataSource::setExpectedLineSize(int expected_line_size)
{
  this->expected_line_size = expected_line_size;
  line_size_fixed = true;
}
