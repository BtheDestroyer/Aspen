#define __TIME_CPP

#include "Time.hpp"

#undef __TIME_CPP

namespace Aspen
{
namespace Time
{
std::chrono::microseconds GetTime()
{
  auto tse = std::chrono::steady_clock::now().time_since_epoch();
  return std::chrono::duration_cast<std::chrono::milliseconds>(tse);
}

Time::Time()
    : Object(nullptr, "Time"), _currentTime(0), _deltaTime(0)
{
  _currentTime = GetTime();
  Log::Debug("%f", _currentTime);
}

Time::~Time()
{
}

void Time::operator()()
{
  _lastTime = _currentTime;
  _currentTime = GetTime();
  _deltaTime = _currentTime - _lastTime;
  Log::Debug("%.6f %.6f %.6f", LastTime(), CurrentTime(), DeltaTime());
}

float Time::LastTime()
{
  return float(_lastTime.count()) / 1000000.0f;
}

float Time::CurrentTime()
{
  return float(_currentTime.count()) / 1000000.0f;
}

float Time::DeltaTime()
{
  return float(_deltaTime.count()) / 1000000.0f;
}
} // namespace Time
} // namespace Aspen
