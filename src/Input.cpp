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
Axis::Axis(Object *parent, std::string name)
    : Axis(SDLK_UNKNOWN, SDLK_UNKNOWN, 0.1f, 0.1f, parent, name)
{
}

Axis::Axis(SDL_Keycode positive, SDL_Keycode negative, float gravity, float weight, Object *parent, std::string name)
    : Object(parent, name), _pos(positive), _neg(negative), _gravity(gravity), _weight(weight), _value(0)
{
}

void Axis::operator()()
{
  Time::Time *time = FindAncestorOfType<Time::Time>();
  if (!time)
    time = FindAncestorOfType<Engine::Engine>()->Time();
  float w = _weight * float(time->DeltaTime() * 60.0);
  float d = KeyHeld(_pos) ? 1.0f : 0.0f + KeyHeld(_neg) ? -1.0f : 0.0f;
  if (d != 0.0f)
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
