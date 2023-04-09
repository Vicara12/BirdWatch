#include "datahandler.h"


DataHandler::DataHandler ()
{
  //
}


void DataHandler::setDataSource (DataSource *data_source)
{
  this->data_source = data_source;
}


void DataHandler::setPFD (PFD *pfd)
{
  this->pfd = pfd;
}


void DataHandler::updateData ()
{
  // TODO
}



void DataHandler::setDataFields (std::vector<std::string> data_fields)
{
  this->data_fields = data_fields;
}
