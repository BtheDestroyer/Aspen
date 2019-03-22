#ifndef __DEBUG_HPP
#define __DEBUG_HPP

#include "imgui.h"
#include "Object.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Debug namespace
namespace Debug
{
class Debug : public Object::Object
{
  ImGuiIO *_io;
  static unsigned _dcount;
  std::vector<const Object *> _toClose;
  std::vector<const Object *> _toOpen;

public:
  Debug(Object *parent = nullptr, std::string name = "Debug");
  void Setup();
  void operator()();
  void MakeTree(Object *o);
  ~Debug();

  void CloseAll();
  void Close(Object *o);
  void Open(Object *o);

  void PopulateDebugger();
};
} // namespace Debug
} // namespace Aspen

#endif
