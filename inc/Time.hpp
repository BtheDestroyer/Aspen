#ifndef __TIME_HPP
#define __TIME_HPP
#include "Object.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Time namespace
namespace Time
{
class Time : public Object::Object
{
  float _lastTime;
  float _currentTime;
  float _deltaTime;
public:
  Time();
  ~Time();

  void operator()();

  float TimeLastUpdate();
  float CurrentTime();
  float DeltaTime();
};
} // namespace Time
} // namespace Aspen

#endif
