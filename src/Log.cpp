#define __LOG_CPP

#include "Log.hpp"
#include <iostream>
#include <iomanip>
#include <stdarg.h>

#undef __LOG_CPP

namespace Gladius
{
namespace Log
{
int Log::_line = 0;

Log::Log(std::string prefix, std::string suffix, bool print)
    : _pre(prefix), _suf(suffix), _print(print) {}

void Log::operator()(const std::string &format, ...)
{
  if (!_print)
    return;
  char buffer[512];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format.c_str(), args);
  va_end(args);
  std::cout << "[" << std::setw(4) << std::setfill('0') << (_line++) << std::setw(0) << "] " << _pre << buffer << _suf << std::endl;
}

void Log::TogglePrint()
{
  _print = !_print;
}

#ifdef __WIN32
#ifdef __DEBUG
Log Debug = Log("DBG: ");
#else
Log Debug = Log("DBG: ", "", false);
#endif
Log Info = Log("INF: ");
Log Warning = Log("WAR: ");
Log Error = Log("ERR: ");
#else
#ifdef __DEBUG
Log Debug = Log("\e[36mDBG: \e[39m");
#else
Log Debug = Log("\e[36mDBG: \e[39m", "", false);
#endif
Log Info = Log("INF: ");
Log Warning = Log("\e[93mWAR: \e[39m");
Log Error = Log("\e[91mERR: \e[39m");
#endif
} // namespace Log
} // namespace Gladius
