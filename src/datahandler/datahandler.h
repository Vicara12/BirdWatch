#ifndef DATA_HANDLER_H_
#define DATA_HANDLER_H_

#include "pfd/pfd.h"
#include "datasource.h"


class DataHandler
{
  public:
    DataHandler ();
    void setDataSource (DataSource *data_source);
    void setDataFields (std::vector<std::string> data_fields);
    void setPFD (PFD *pfd);
    void updateData ();

  private:
    PFD *pfd;
    DataSource *data_source;
    std::vector<std::string> data_fields;
};

#endif
