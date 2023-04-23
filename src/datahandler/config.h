#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <nlohmann/json.hpp>

class Config
{
public:

  Config ();
  bool parseConfigFile (std::string path);
  bool getStringField (const std::string field_name, std::string &data);
  bool getNumericField (const std::string field_name, std::string &data);

private:


  nlohmann::json config_json;
};

#endif
