#define __TIME_CPP

#include "Time.hpp"
#include <chrono>

#undef __TIME_CPP

namespace Aspen
{
namespace Time
{
float TimeInSeconds()
{
  return std::chrono::duration<float>(std::chrono::system_clock::now().time_since_epoch()).count();
}

Time::Time()
    : Object(nullptr, "Time"), _currentTime(0), _deltaTime(0)
{
  _lastTime = TimeInSeconds();
}

Time::~Time()
{
}

void Time::operator()()
{
  float temp = _currentTime;
  _currentTime = TimeInSeconds();
  _deltaTime = _currentTime - _lastTime;
  _lastTime = temp;
  Log::Debug("%f", _deltaTime);
}

float Time::TimeLastUpdate()
{
  return _lastTime;
}

float Time::CurrentTime()
{
  return _currentTime;
}

float Time::DeltaTime()
{
  return _deltaTime;
}
} // namespace Time
} // namespace Aspen
