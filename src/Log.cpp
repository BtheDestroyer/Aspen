#define __LOG_CPP
#define _CRT_SECURE_NO_WARNINGS
#include "Log.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdarg.h>

#undef __LOG_CPP

namespace Aspen
{
namespace Log
{
int Log::_line = 0;
std::fstream Log::_file;

Log::Log(std::string prefix, std::string suffix, bool print)
    : _pre(prefix), _suf(suffix), _print(print) {}

void Log::operator()(const char *format, ...)
{
  if (!_print)
    return;
  char buffer[512];
  va_list args;
  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  std::stringstream output;
  output << "[" << std::setw(4) << std::setfill('0') << (_line++) << std::setw(0) << "] " << _pre << buffer << _suf << std::endl;
  std::cout << output.str();
  if (_file.is_open())
    _file << output.str();
}

void Log::TogglePrint()
{
  _print = !_print;
}

bool Log::SetFile(std::string path)
{
  _file.open(path.c_str(), std::ios_base::out | std::ios_base::app | std::ios_base::ate);
  return _file.is_open();
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
} // namespace Aspen
