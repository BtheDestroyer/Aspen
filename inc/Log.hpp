#ifndef __LOG_HPP
#define __LOG_HPP
#include <string>
#include <sstream>

namespace Gladius
{
namespace Log
{
class Log
{
  static int _line;
  std::string _pre;
  std::string _suf;
  bool _print;

public:
  Log(std::string prefix = "", std::string suffix = "", bool print = true);
  void operator()(const std::string &format, ...);
  void operator()(const std::stringstream &message) { operator()(message.str()); }
  void TogglePrint();
};

extern Log Debug;
extern Log Info;
extern Log Warning;
extern Log Error;
} // namespace Log
} // namespace Gladius

#endif
