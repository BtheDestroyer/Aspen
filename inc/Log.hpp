#ifndef __LOG_HPP
#define __LOG_HPP
#include <string>
#include <sstream>

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Log namespace
///        Contains Log class and references to external instantiations.
namespace Log
{
/// \brief Log functor class
///        Used for logging information to the console
class Log
{
  /// \brief Current line number
  static int _line;
  /// \brief Prefix used when logging
  std::string _pre;
  /// \brief Suffix used when logging
  std::string _suf;
  /// \brief Determines if calls to operator() are completed
  bool _print;
  /// \brief Output filestream shared across all Log classes
  static std::fstream _file;

public:
  /// \brief Constructor
  /// \param prefix Prefix used when logging
  /// \param suffix Suffix used when logging
  /// \param print Determines if calls to operator() are completed
  Log(std::string prefix = "", std::string suffix = "", bool print = true);
  /// \brief Log request
  ///        Functions similarly to printf
  /// \param format Format string to be passed to vsprintf
  void operator()(const char *format, ...);
  /// \brief Log request
  /// \param message Message to be unwrapped as a string
  void operator()(const std::stringstream &message) { operator()(message.str().c_str()); }
  /// \brief Toggles _print
  void TogglePrint();

  /// \brief sets the current logfile
  /// \param path Output filepath
  /// \return Success of opening the output file
  static bool SetFile(std::string path);
};

/// \brief Used for logging debug info.
extern Log Debug;
/// \brief Used for logging general info.
extern Log Info;
/// \brief Used for logging warnings.
extern Log Warning;
/// \brief Used for logging errors.
extern Log Error;
} // namespace Log
} // namespace Aspen

#endif
