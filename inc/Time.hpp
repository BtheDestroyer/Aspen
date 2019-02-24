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
class Time : public Object::Object
{
  std::chrono::microseconds _startTime;
  std::chrono::microseconds _lastTime;
  std::chrono::microseconds _currentTime;
  std::chrono::microseconds _deltaTime;

public:
  Time();
  ~Time();

  void operator()();

  double StartTime();
  double LastTime();
  double CurrentTime();
  double DeltaTime();
};
} // namespace Time
} // namespace Aspen

#endif
