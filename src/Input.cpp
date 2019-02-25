#define __INPUT_CPP

#include "Input.hpp"
#include "Time.hpp"
#include "Engine.hpp"
#include <map>

#undef __INPUT_CPP

namespace Aspen
{
namespace Input
{
Axis::Axis(Key posiive, Key negative, float gravity, float weight)
    : _pos(positive), _neg(negative), _gravity(gravity), _weight(weight), _value(0)
{
}

void Axis::operator()()
{
  Time *time = FindAncestorOfType<Time::Time>();
  if (!time)
    time = FindAncestorOfType<Engine::Engine>()->Time();
  float w = _weight * float(time->DeltaTime() * 60.0);
  float d = KeyHeld(_pos) ? 1 : 0 + KeyHeld(_neg) ? -1 : 0;
  if (d != 0)
    _value = _value * (1 - w) + d * w;
  else
    _value *= 1 - _gravity;
}

float Axis::GetValue()
{
  return _value;
}

std::map<SDL_Keycode, Key> keys;

Key::Key()
    : held(false), pressed(false), released(false)
{
}

Key &GetKey(SDL_Keycode k)
{
  if (keys.find(k) == keys.end())
    keys[k] = Key();
  return keys[k];
}

bool KeyHeld(SDL_Keycode k)
{
  return GetKey(k).held;
}

bool KeyPressed(SDL_Keycode k)
{
  return GetKey(k).pressed;
}

bool KeyReleased(SDL_Keycode k)
{
  return GetKey(k).released;
}
} // namespace Input
} // namespace Aspen
