#define __TIME_CPP

#include "Time.hpp"
#ifdef __LINUX
#include <thread>
#endif
#ifdef __WIN32
#include <windows.h>
#endif

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

double Time::FPS()
{
  return 1.0 / DeltaTime();
}

void Time::Sleep(double time)
{
#ifdef __LINUX
  std::this_thread::sleep_for(std::chrono::microseconds(long long(time * 1000000)));
#endif
#ifdef __WIN32
  ::Sleep(DWORD(time * 1000));
#endif
}

void Time::Sleep(float time)
{
#ifdef __LINUX
  std::this_thread::sleep_for(std::chrono::microseconds(long long(time * 1000000)));
#endif
#ifdef __WIN32
  ::Sleep(DWORD(time * 1000));
#endif
}
} // namespace Time
} // namespace Aspen
