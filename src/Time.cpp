#define __TIME_CPP

#include "Time.hpp"
#ifdef __LINUX
#include <thread>
#endif
#ifdef __WIN32
#include <windows.h>
#endif
#include "imgui.h"

#undef __TIME_CPP

namespace Aspen
{
namespace Time
{
Time *Time::_main = nullptr;

std::chrono::microseconds GetTime()
{
  auto tse = std::chrono::steady_clock::now().time_since_epoch();
  return std::chrono::duration_cast<std::chrono::microseconds>(tse);
}

Time::Time(Object *parent, std::string name)
    : Time(0xFFFFFFFF, parent, name)
{
}

Time::Time(unsigned targetFramerate, Object *parent, std::string name)
    : Object(parent, name), _deltaTime(0), _targetFramerate(targetFramerate)
{
  _startTime = _lastTime = _currentTime = GetTime();
}

Time::~Time()
{
}

Time *Time::Get()
{
  return _main;
}

void Time::operator()()
{
  if (!Active())
    return;
  _lastTime = _currentTime;
  _currentTime = GetTime();
  _deltaTime = _currentTime - _lastTime;
  if (FPS() > double(_targetFramerate))
  {
    Sleep((1.0 / double(_targetFramerate)) - DeltaTime());
    _currentTime = GetTime();
    _deltaTime = _currentTime - _lastTime;
  }
  Object::operator()();
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
  if (_targetFramerate > 0)
    return std::min(std::max(0.0, double(_deltaTime.count()) / 1000000.0), 1.0 / _targetFramerate);
  return std::max(0.0, double(_deltaTime.count()) / 1000000.0);
}

double Time::FPS()
{
  double r = 1.0 / std::max(std::max(0.0, double(_deltaTime.count()) / 1000000.0), 0.00000001);
  return r;
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

unsigned Time::TargetFramerate()
{
  return _targetFramerate;
}

void Time::TargetFramerate(unsigned targetFramerate)
{
  _targetFramerate = targetFramerate;
}

void Time::PopulateDebugger()
{
  ImGui::Text("Start Time: %f", StartTime());
  ImGui::Text("Last Time: %f", LastTime());
  ImGui::Text("Current Time: %f", CurrentTime());
  ImGui::Text("Delta Time: %f", DeltaTime());
  int tf = int(_targetFramerate);
  ImGui::InputInt("Target Framerate", &tf, 1, 1);
  if (std::abs(tf - int(_targetFramerate)) >= 1)
    _targetFramerate = unsigned(tf);
  Object::PopulateDebugger();
}
} // namespace Time
} // namespace Aspen
