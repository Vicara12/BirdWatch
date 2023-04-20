#ifndef DATA_HANDLER_H_
#define DATA_HANDLER_H_

#include "pfd/pfd.h"
#include "datasource.h"
#include <map>


// supported data items
//
//    * YPR: yaw, pitch and roll, all of the three must be provided concecutively
//           and in that order


class DataHandler
{
  public:
    DataHandler ();
    void setDataSource (DataSource *data_source);
    DataSource* getDataSource ();
    void setDataFields (const std::vector<std::string> &data_fields);
    void setPFD (PFD *pfd);
    void updateData ();
    void printDataInTerminal (bool print);
    void setNoDataTimeout (long milliseconds);
    // tells if data has been received in the last x milliseconds (with x = noDataTimeout)
    bool checkDataLink ();
    bool thereIsMoreData () const;

  private:
    PFD *pfd;
    DataSource *data_source;
    std::map<std::string, int> data_format;
    bool pfd_provided;
    bool print_in_term;
    long no_data_timeout;
};

#endif
