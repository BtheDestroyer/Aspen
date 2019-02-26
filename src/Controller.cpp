#define __CONTROLLER_CPP

#include "Controller.hpp"
#include "Engine.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include "Input.hpp"
#include <cmath>
#include "imgui.h"

#undef __CONTROLLER_CPP

namespace Aspen
{
namespace Controller
{
PlayerController_8Way::PlayerController_8Way(Object *parent, std::string name)
    : PlayerController_8Way(SDLK_s, SDLK_w, SDLK_a, SDLK_d, 4.0, parent, name)
{
}

PlayerController_8Way::PlayerController_8Way(SDL_Keycode up, SDL_Keycode down, SDL_Keycode left, SDL_Keycode right, double speed, Object *parent, std::string name)
    : Object(parent, "PlayerController_8Way"), _speed(speed)
{
  AddChild(new Input::Axis(up, down, 0.1f, 0.1f, this, "Axis-Vertical"));
  AddChild(new Input::Axis(right, left, 0.1f, 0.1f, this, "Axis-Horizontal"));
}

void PlayerController_8Way::operator()()
{
  if (!_parent)
    return;
  Transform::Transform *tf = dynamic_cast<Transform::Transform *>(_parent);
  if (!tf)
    tf = _parent->FindChildOfType<Transform::Transform>();
  if (!tf)
    return;

  Object::operator()();

  //TODO: Replace with Physics simulation
  Input::Axis *av = nullptr;
  Input::Axis *ah = nullptr;
  for (Input::Axis *a : FindChildrenOfType<Input::Axis>())
  {
    if (!av && a->Name() == "Axis-Vertical")
      av = a;
    else if (!ah && a->Name() == "Axis-Horizontal")
      ah = a;
    if (ah && av)
      break;
  }
  if (!ah || !av)
  {
    End();
    return;
  }
  Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();
  Time::Time *time = nullptr;
  if (engine)
    time = engine->FindChildOfType<Time::Time>();
  if (time)
  {
    double dx = ah->GetValue() * _speed * time->DeltaTime() * 60;
    double dy = av->GetValue() * _speed * time->DeltaTime() * 60;
    tf->ModifyPosition(dx, dy);
  }
  else
    tf->ModifyPosition(ah->GetValue() * _speed, av->GetValue() * _speed);
}

void PlayerController_8Way::Speed(double speed)
{
  _speed = speed;
}

double PlayerController_8Way::Speed()
{
  return _speed;
}

void PlayerController_8Way::PopulateDebugger()
{
  ImGui::Text("Speed: %f", _speed);
  Object::PopulateDebugger();
}
} // namespace Controller
} // namespace Aspen
