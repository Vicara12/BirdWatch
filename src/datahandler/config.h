#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <nlohmann/json.hpp>

class Config
{
public:

  Config ();
  bool parseConfigFile (std::string path);
  bool hasField (const std::string &field_name);
  bool getField (const std::string &field_name, std::string &data);
  bool getField (const std::string &field_name, int &data);
  bool getField (const std::string &field_name, bool &data);
  bool getField (const std::string &field_name, std::vector<std::string> &data);
  bool getDataSourceField (const std::string &field_name, std::string &data);
  bool getDataSourceField (const std::string &field_name, int &data);

private:


  nlohmann::json config_json;
};

#endif
