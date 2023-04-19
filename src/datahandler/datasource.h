#ifndef DATA_SOURCE_H_
#define DATA_SOURCE_H_

#include <string>
#include <vector>

enum DataFormat { DF_BINARY, DF_ASCII };

class DataSource
{
  public:
    DataSource ();
    virtual ~DataSource ();
    virtual bool newDataAvailable () = 0;
    virtual std::vector<float> getLatestData () = 0;
    virtual bool initOk () const = 0;
    void setDataFormat (DataFormat data_format);
    void setFieldsPerLine (int expected_line_size);
    void setAsciiDataSeparator (char separator);
    long long getTimeLastData () const;

  protected:
    long long getMillis () const;

    DataFormat data_format;
    bool line_size_fixed;
    int expected_line_size;
    char separator;
    long long time_last_data_read; // classes that inherit from this must
                                   // keep this value up to date
};

#endif
