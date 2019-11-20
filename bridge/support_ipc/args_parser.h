#pragma once

#include <map>
#include <string>
#include <vector>


namespace support_ipc {
class Args
{
public:
  bool Exist(const std::string &key) const;
  std::string &operator[](const std::string &key);
  size_t Size() const;
  std::vector<std::string> Keys();

private:
  /**
    * \brief Command line parameter key value pair.
    *
    * key:
    * value
    */
  std::map<std::string, std::string> _args;
};

class ArgsParser
{
public:
  /**
    * \brief Mapping after command line parameter parsing.
    *
    * exp: "-speed 1"   _args["speed"] = "1"
    * exp: "-stop",     _args["stop"] = ""
    *
    */
  static Args ParseArgs(int argc, wchar_t *argv[]);

  static Args ParseArgs(int argc, char *argv[]);
};
}
