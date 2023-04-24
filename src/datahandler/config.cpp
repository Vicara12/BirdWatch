#include <iostream>
#include <fstream>
#include "config.h"

using json = nlohmann::json;

Config::Config ()
{
}


bool Config::parseConfigFile (std::string path)
{
  std::ifstream config_file(path);

  if (not config_file.is_open()) {
    std::cout << "ERROR: could not open config file \"" << path << "\"" << std::endl;
    return false;
  }

  try {
    config_file >> config_json;
  } catch (json::exception &e) {
    std::cout << "ERROR: could not parse config file:" << std::endl
              << e.what() << std::endl;
    return false;
  }

  // check data_source field exists
  if (not config_json.contains("data_source")) {
    std::cout << "ERROR: config file does not contain the data_source field" << std::endl;
    return false;
  }

  return true;
}


bool Config::hasField (const std::string &field_name)
{
  return config_json.contains(field_name);
}


bool Config::getField (const std::string &field_name, std::string &data)
{
  if (not config_json.contains(field_name))
      return false;
  data = config_json[field_name];
  return true;
}


bool Config::getField (const std::string &field_name, int &data)
{
  if (not config_json.contains(field_name))
      return false;
  data = config_json[field_name];
  return true;
}


bool Config::getField (const std::string &field_name, bool &data)
{
  if (not config_json.contains(field_name))
      return false;
  data = config_json[field_name];
  return true;
}


bool Config::getField (const std::string &field_name, std::vector<std::string> &data)
{
  if (not config_json.contains(field_name))
      return false;
  data.clear();
  for (auto item : config_json[field_name].items())
    data.push_back(item.value());
  return true;
}


bool Config::getDataSourceField (const std::string &field_name, std::string &data)
{
  if (not config_json["data_source"].contains(field_name))
      return false;
  data = config_json["data_source"][field_name];
  return true;
}


bool Config::getDataSourceField (const std::string &field_name, int &data)
{
  if (not config_json["data_source"].contains(field_name))
      return false;
  data = config_json["data_source"][field_name];
  return true;
}
