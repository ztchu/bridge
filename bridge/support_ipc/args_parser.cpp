#include "args_parser.h"

#include "tools.h"

namespace support_ipc
{
bool Args::Exist(const std::string &key) const
{
  return _args.find(key) != _args.end();
}

std::string &Args::operator[](const std::string &key)
{
  return _args[key];
}

size_t Args::Size() const
{
  return _args.size();
}

std::vector<std::string> Args::Keys()
{
  std::vector<std::string> keys;
  for (auto itor = _args.begin(); itor != _args.end(); ++itor)
  {
    keys.push_back(itor->first);
  }
  return keys;
}

Args ArgsParser::ParseArgs(int argc, wchar_t *argv[])
{
  Args args;
  for (auto i = 0; i < argc; ++i)
  {
    std::string value = _TO_UTF8(argv[i]);
    if (value[0] == '-' && ++i < argc)
    {
      std::string next = _TO_UTF8(argv[i]);
      if (!next.empty() && next[0] == '-')
      {
        i--;
        args[value] = "";
      }
      else
      {
        args[value] = next;
      }
    }
    else
    {
      args[value] = "";
    }
  }

  return args;
}

Args ArgsParser::ParseArgs(int argc, char *argv[])
{
  Args args;
  for (auto i = 0; i < argc; ++i)
  {
    std::string value = argv[i];
    if (value[0] == '-' && ++i < argc)
    {
      std::string next = argv[i];
      if (!next.empty() && next[0] == '-')
      {
        i--;
        args[value] = "";
      }
      else
      {
        args[value] = next;
      }
    }
    else
    {
      args[value] = "";
    }
  }

  return args;
}
} // namespace rtmp_helper