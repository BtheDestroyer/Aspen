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
/// \brief Debugger wrapper class for ImGui
class Debug : public Object::Object
{
  /// \brief ImGuiIO
  ImGuiIO *_io;
  /// \brief Total number of Debugger Objects
  static unsigned _dcount;
  /// \brief Objects to close in the Object tree
  std::vector<const Object *> _toClose;
  /// \brief Objects to open in the Object tree
  std::vector<const Object *> _toOpen;

  /// \brief Sets up ImGui
  void Setup();

public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Debug(Object *parent = nullptr, std::string name = "Debug");
  /// \brief Destructor
  ~Debug();

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Creates the Object tree for the given object
  /// \param o Object to the debugger tree for
  void MakeTree(Object *o);

  /// \brief Closes every tree node in the Object tree
  void CloseAll();
  /// \brief Closes the tree node in the Object tree for the given Object
  /// \param o Object of which the tree node will be closed
  void Close(Object *o);
  /// \brief Opens the tree node in the Object tree for the given Object
  /// \param o Object of which the tree node will be opened
  void Open(Object *o);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};
} // namespace Debug
} // namespace Aspen

#endif
