#include "datahandler.h"


DataHandler::DataHandler () :
  pfd_provided(false),
  print_in_term(false),
  no_data_timeout(250),
  data_source_provided(false)
{
  data_format["YPR"] = -1;
}


void DataHandler::setDataSource (DataSource *data_source)
{
  data_source_provided = true;
  this->data_source = data_source;
}


void DataHandler::setPFD (PFD *pfd)
{
  this->pfd = pfd;
  pfd_provided = true;
}


void DataHandler::updateData ()
{
  if (data_source->newDataAvailable()) {
    std::vector<float> new_data = data_source->getLatestData();
    if (print_in_term) {
      for (float item : new_data)
        std::cout << item << " ";
      std::cout << std::endl;
    }
    int ypr_pos = data_format["YPR"];
    if (ypr_pos != -1)
      pfd->setYPR(new_data[ypr_pos], new_data[ypr_pos+1], new_data[ypr_pos+2]);
  }
}


void DataHandler::setDataFields (const std::vector<std::string> &data_fields)
{
  int field_counter = 0;
  for (std::string field : data_fields) {
    if (field == "video") // video input is not handled by this class
      continue;
    std::map<std::string, int>::iterator key_pos = data_format.find(field);
    if (key_pos == data_format.end())
      std::cout << "WARNING: unkwnown data field \"" << field << "\"" << std::endl;
    else {
      data_format[field] = field_counter;
      if (field == "YPR") {
        field_counter += 3;
        if (not pfd_provided)
          std::cout << "WARNING: setting YPR attribute but PFD not provided yet" << std::endl;
      }
      else
        std::cout << "ERROR: field \"" << field << "\" does not have a "
                  << "defined number of items" << std::endl;
    }
  }

  if (not data_source_provided) {
    std::cout << "ERROR: trying to set data fields before providing a data source "
              << "is not suported" << std::endl;
  }
  data_source->setFieldsPerLine(field_counter);
}


void DataHandler::printDataInTerminal(bool print)
{
  print_in_term = print;
}


void DataHandler::setNoDataTimeout (long milliseconds)
{
  no_data_timeout = milliseconds;
}


bool DataHandler::checkDataLink ()
{
  return data_source->getTimeLastData() < no_data_timeout;
}


DataSource* DataHandler::getDataSource ()
{
  return data_source;
}


bool DataHandler::thereIsMoreData () const
{
  return data_source->thereIsMoreData();
}
