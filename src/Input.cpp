#define __INPUT_CPP

#include "Input.hpp"
#include "Time.hpp"
#include "Engine.hpp"
#include <cmath>
#include <map>
#include "imgui.h"

#undef __INPUT_CPP

namespace Aspen
{
namespace Input
{
Axis::Axis(Object *parent, std::string name)
    : Axis(SDLK_UNKNOWN, SDLK_UNKNOWN, 0.5f, 0.5f, parent, name)
{
}

Axis::Axis(SDL_Keycode positive, SDL_Keycode negative, float gravity, float weight, Object *parent, std::string name)
    : Object(parent, name), _pos(positive), _neg(negative), _gravity(gravity), _weight(weight), _value(0)
{
}

void Axis::operator()()
{
  Time::Time *time = nullptr;
  Engine::Engine *engine = Engine::Engine::Get();
  if (engine)
    time = Time::Time::Get();
  if (!time) 
  {
    Log::Error("Axis Object can't find an Engine ancestor with a Time child or a Time ancestor!");
    return;
  }
  float d = (KeyHeld(_pos) ? 1.0f : 0.0f) + (KeyHeld(_neg) ? -1.0f : 0.0f);
  if (d != 0.0f)
  {
    float w = std::min(1.0f, _weight * float(time->DeltaTime() * 60));
    _value = _value * (1.0f - w) + d * w;
  }
  else
    _value *= 1.0f - std::min(1.0f, _gravity * float(time->DeltaTime() * 60));
}

float Axis::GetValue()
{
  return _value;
}

void Axis::PopulateDebugger()
{
  ImGui::Text("Positive Key: %s", SDL_GetKeyName(_pos));
  ImGui::Text("Negative Key: %s", SDL_GetKeyName(_neg));
  ImGui::DragFloat("Gravity", &_gravity, 0.1f);
  ImGui::DragFloat("Weight", &_weight, 0.1f);
  ImGui::Text("Value: %f", GetValue());
  Object::PopulateDebugger();
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

Mouse mouse;

Mouse &GetMouse()
{
  return mouse;
}
} // namespace Input
} // namespace Aspen
