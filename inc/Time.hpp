#ifndef __TIME_HPP
#define __TIME_HPP
#include "Object.hpp"
#include <chrono>

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Time namespace
namespace Time
{
/// \brief Time management class
class Time : public Object::Object
{
  /// \brief Time the object was created
  std::chrono::microseconds _startTime;
  /// \brief Time the last frame started
  std::chrono::microseconds _lastTime;
  /// \brief Time the current frame started
  std::chrono::microseconds _currentTime;
  /// \brief Time between the last frame and now
  ///        Max: 1 / _targetFramerate
  std::chrono::microseconds _deltaTime;
  /// \brief Target framerate
  ///        Typically refresh rate of memory
  unsigned _targetFramerate;
  /// \brief First created Time object
  static Time *_main;

public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Time(Object *parent = nullptr, std::string name = "Time");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param targetFramerate Target framerate
  ///                        Typically refresh rate of memory
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Time(unsigned targetFramerate, Object *parent = nullptr, std::string name = "Time");
  /// \brief Destructor
  ~Time();

  /// \brief Gets the main time object
  /// \return _main
  static Time *Get();

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Start time of the application in seconds
  /// \return Start time of the application
  double StartTime();
  /// \brief Start time of the last frame in seconds
  /// \return Start time of the last frame
  double LastTime();
  /// \brief Start time of the current frame in seconds
  /// \return Start time of the current frame
  double CurrentTime();
  /// \brief Time since the last frame in seconds
  /// \return Time since the last frame
  double DeltaTime();
  /// \brief Current framerate of the application
  /// \return Current framerate of the application
  double FPS();

  /// \brief Sleeps for a set amount of time
  /// \param time Time to sleep in seconds
  void Sleep(double time);
  /// \brief Sleeps for a set amount of time
  /// \param time Time to sleep in seconds
  void Sleep(float time);

  /// \brief Gets the target framerate
  /// \return _targetFramerate
  unsigned TargetFramerate();
  /// \brief Sets the target framerate
  /// \param targetFramerate New target framerate
  void TargetFramerate(unsigned targetFramerate);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};
} // namespace Time
} // namespace Aspen

#endif
