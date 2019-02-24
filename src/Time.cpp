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
  return std::chrono::duration_cast<std::chrono::microseconds>(tse);
}

Time::Time()
    : Object(nullptr, "Time"), _deltaTime(0)
{
  _startTime = _currentTime = GetTime();
}

Time::~Time()
{
}

void Time::operator()()
{
  _lastTime = _currentTime;
  _currentTime = GetTime();
  _deltaTime = _currentTime - _lastTime;
  Log::Debug("last: %f %lld", LastTime(), _lastTime.count());
  Log::Debug("curr: %f %lld", CurrentTime(), _currentTime.count());
  Log::Debug("delt: %f %lld", DeltaTime(), _deltaTime.count());
}

double Time::StartTime()
{
  return double(_startTime.count()) / 1000000.0;
}

double Time::LastTime()
{
  return double(_lastTime.count()) / 1000000.0;
}

double Time::CurrentTime()
{
  return double(_currentTime.count()) / 1000000.0;
}

double Time::DeltaTime()
{
  return double(_deltaTime.count()) / 1000000.0;
}
} // namespace Time
} // namespace Aspen
