#define __CONTROLLER_CPP

#include "Controller.hpp"
#include "Engine.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include "Physics.hpp"
#include "Input.hpp"
#include <cmath>
#include "imgui.h"

#undef __CONTROLLER_CPP

namespace Aspen
{
namespace Controller
{
PlayerController_8Way::PlayerController_8Way(Object *parent, std::string name)
    : PlayerController_8Way(SDLK_s, SDLK_w, SDLK_a, SDLK_d, 1.0, parent, name)
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
  {
    Log::Error("%s needs a parent!", Name().c_str());
    return;
  }
  Physics::Rigidbody *rb = _parent->FindChildOfType<Physics::Rigidbody>();
  Transform::Transform *tf = nullptr;
  if (rb)
  {
    tf = _parent->FindChildOfType<Transform::Transform>();
    if (!tf)
    {
      Log::Error("%s needs a parent with a child Transform::Transform!", Name().c_str());
      return;
    }
  }
  else
  {
    Log::Warning("%s works better with a parent with a child Physics::Rigidbody!", Name().c_str());
    return;
  }

  Object::operator()();

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
    Log::Error("%s requires two children of type Axis named Axis-Vertical and Axis-Horizontal!", Name().c_str());
    return;
  }
  double dx = ah->GetValue() * _speed;
  double dy = av->GetValue() * _speed;
  if (!rb)
    tf->ModifyPosition(dx, dy);
  else
    rb->SetCartesianAcceleration(dx, dy);
}

void PlayerController_8Way::SetSpeed(double speed)
{
  _speed = speed;
}

double PlayerController_8Way::GetSpeed()
{
  return _speed;
}

void PlayerController_8Way::PopulateDebugger()
{
  ImGui::Text("Speed: %f", _speed);
  if (_parent)
  {
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
    if (av && ah)
    {
      double dx = ah->GetValue() * _speed;
      double dy = av->GetValue() * _speed;
      ImGui::Text("Input: (%.3f, %.3f)", dx, dy);
    }
  }
  Object::PopulateDebugger();
}

/////////////////////////////////////////////////////////

PlayerController_Sidescroller::PlayerController_Sidescroller(Object *parent, std::string name)
    : PlayerController_Sidescroller(SDLK_a, SDLK_d, SDLK_w, 1.0, 2.0, parent, name)
{
}

PlayerController_Sidescroller::PlayerController_Sidescroller(SDL_Keycode left, SDL_Keycode right, SDL_Keycode jump, double speed, double jumpStrength, Object *parent, std::string name)
    : Object(parent, name), _speed(speed), _jumpStrength(jumpStrength), _jumpKey(jump)
{
  AddChild(new Input::Axis(right, left, 0.1f, 0.1f, this, "Axis-Horizontal"));
}

void PlayerController_Sidescroller::operator()()
{
  if (!_parent)
  {
    Log::Error("%s needs a parent!", Name().c_str());
    return;
  }
  Physics::Rigidbody *rb = _parent->FindChildOfType<Physics::Rigidbody>();
  Transform::Transform *tf = nullptr;
  if (rb)
  {
    tf = _parent->FindChildOfType<Transform::Transform>();
    if (!tf)
    {
      Log::Error("%s needs a parent with a child Transform::Transform!", Name().c_str());
      return;
    }
  }
  else
  {
    Log::Warning("%s works better with a parent with a child Physics::Rigidbody!", Name().c_str());
    return;
  }

  Object::operator()();

  Input::Axis *ah = nullptr;
  for (Input::Axis *a : FindChildrenOfType<Input::Axis>())
  {
    if (!ah && a->Name() == "Axis-Horizontal")
    {
      ah = a;
      break;
    }
  }
  if (!ah)
  {
    Log::Error("%s requires a child of type Axis named Axis-Horizontal!", Name().c_str());
    return;
  }
  double dx = ah->GetValue() * _speed;
  if (!rb)
    tf->ModifyPosition(dx, Input::KeyPressed(_jumpKey) ? -1 * _jumpStrength : 0.0);
  else
  {
    rb->SetCartesianAcceleration(dx, 0);
    if (Input::KeyPressed(_jumpKey))
      rb->SetCartesianVelocity(rb->GetVelocityX(), -0.5 * _jumpStrength * _jumpStrength);
  }
}

void PlayerController_Sidescroller::SetSpeed(double speed)
{
  _speed = speed;
}

double PlayerController_Sidescroller::GetSpeed()
{
  return _jumpStrength;
}

void PlayerController_Sidescroller::SetJumpStrength(double strength)
{
  _jumpStrength = strength;
}

double PlayerController_Sidescroller::GetJumpStrength()
{
  return _jumpStrength;
}

void PlayerController_Sidescroller::PopulateDebugger()
{
  ImGui::Text("Speed: %f", _speed);
  ImGui::Text("Jump Strength: %f", _jumpStrength);
  if (_parent)
  {
    Input::Axis *av = nullptr;
    Input::Axis *ah = nullptr;
    for (Input::Axis *a : FindChildrenOfType<Input::Axis>())
    {
      if (!ah && a->Name() == "Axis-Horizontal")
      {
        ah = a;
        break;
      }
    }
    if (av)
    {
      double dx = ah->GetValue() * _speed;
      ImGui::Text("Input: %.3f", dx);
    }
    ImGui::Text("Jumping?: %s", Input::KeyPressed(_jumpKey) ? "True" : "False");
  }
  Object::PopulateDebugger();
}

} // namespace Controller
} // namespace Aspen
