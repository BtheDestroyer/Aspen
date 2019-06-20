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
int Sign(double x)
{
  return (x > 0) ? 1 : ((x < 0) ? -1 : 0);
}

namespace Controller
{
PlayerController_8Way::PlayerController_8Way(Object *parent, std::string name)
    : PlayerController_8Way(SDLK_s, SDLK_w, SDLK_a, SDLK_d, 4.0, 1.0, parent, name)
{
}

PlayerController_8Way::PlayerController_8Way(SDL_Keycode up, SDL_Keycode down, SDL_Keycode left, SDL_Keycode right, double speed, double acceleration, Object *parent, std::string name)
    : Object(parent, "PlayerController_8Way"), _acceleration(acceleration), _speed(speed)
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
  Physics::Rigidbody *rb = _parent->GetRigidbody();
  Transform::Transform *tf = nullptr;
  if (rb)
  {
    tf = _parent->GetTransform();
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
  double dx = ah->GetValue() * _acceleration;
  double dy = av->GetValue() * _acceleration;
  if (!rb)
    tf->ModifyPosition(dx, dy);
  else
    rb->ApplyCartesianForce((std::abs(rb->GetVelocityX()) < _speed || Sign(rb->GetVelocityX()) != Sign(dx)) ? dx : 0,
                            (std::abs(rb->GetVelocityY()) < _speed || Sign(rb->GetVelocityY()) != Sign(dy)) ? dy : 0);
}

void PlayerController_8Way::SetAcceleration(double acc)
{
  _acceleration = acc;
}

double PlayerController_8Way::GetAcceleration()
{
  return _acceleration;
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
  static float a; a = _acceleration;
  static float s; s = _speed;
  ImGui::DragFloat("Acceleration", &a, 0.1f);
  _acceleration = a;
  ImGui::DragFloat("Speed", &s, 0.1f);
  _speed = s;
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
      double dx = ah->GetValue() * _acceleration;
      double dy = av->GetValue() * _acceleration;
      ImGui::Text("Input X: %.3f", dx);
      ImGui::Text("Input Y: %.3f", dy);
    }
  }
  Object::PopulateDebugger();
}

/////////////////////////////////////////////////////////

PlayerController_Sidescroller::PlayerController_Sidescroller(Object *parent, std::string name)
    : PlayerController_Sidescroller(SDLK_a, SDLK_d, SDLK_w, 4.0, 1.0, 2.0, 5.0, parent, name)
{
}

PlayerController_Sidescroller::PlayerController_Sidescroller(SDL_Keycode left, SDL_Keycode right, SDL_Keycode jump, double speed, double acceleration, double jumpStrength, double jumpHeight, Object *parent, std::string name)
    : Object(parent, name), _acceleration(acceleration), _speed(speed), _jumpStrength(jumpStrength), _jumpHeight(jumpHeight), _jumpRemaining(0), _jumpKey(jump)
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
  Physics::Rigidbody *rb = _parent->GetRigidbody();
  Transform::Transform *tf = nullptr;
  if (rb)
  {
    tf = _parent->GetTransform();
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

  double dt = 1.0;
  Engine::Engine *engine = Engine::Engine::Get();
  if (engine)
  {
    Time::Time *time = Time::Time::Get();
    if (time)
      dt = time->DeltaTime() * 60;
  }

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
  double dx = ah->GetValue() * _acceleration * dt;
  if (!rb)
    tf->ModifyPosition(dx, Input::KeyPressed(_jumpKey) ? -1 * _jumpStrength : 0.0);
  else
  {
    double vx = rb->GetVelocityX();
    if (std::abs(vx) < _speed || Sign(vx) != Sign(dx))
      rb->ApplyCartesianForce(dx, 0);
    if (Input::KeyPressed(_jumpKey))
      _jumpRemaining = _jumpHeight;
    if (_jumpRemaining > 0)
    {
      if (Input::KeyReleased(_jumpKey))
        _jumpRemaining = 0.0;
      else
        _jumpRemaining -= dt / 60.0;
      rb->SetCartesianVelocity(rb->GetVelocityX(), -0.5 * _jumpStrength * _jumpStrength * dt);
    }
  }
}

void PlayerController_Sidescroller::SetAcceleration(double acc)
{
  _acceleration = acc;
}

double PlayerController_Sidescroller::GetAcceleration()
{
  return _acceleration;
}

void PlayerController_Sidescroller::SetSpeed(double speed)
{
  _speed = speed;
}

double PlayerController_Sidescroller::GetSpeed()
{
  return _speed;
}

void PlayerController_Sidescroller::SetJumpStrength(double strength)
{
  _jumpStrength = strength;
}

double PlayerController_Sidescroller::GetJumpStrength()
{
  return _jumpStrength;
}

void PlayerController_Sidescroller::SetJumpHeight(double height)
{
  _jumpHeight = height;
}

double PlayerController_Sidescroller::GetJumpHeight()
{
  return _jumpHeight;
}

void PlayerController_Sidescroller::PopulateDebugger()
{
  static float a; a = _acceleration;
  static float s; s = _speed;
  static float js; js = _jumpStrength;
  static float jh; jh = _jumpHeight;
  ImGui::DragFloat("Acceleration", &a, 0.1f);
  _acceleration = a;
  ImGui::DragFloat("Speed", &s, 0.1f);
  _speed = s;
  ImGui::DragFloat("Jump Strength", &js, 0.1f);
  _jumpStrength = js;
  ImGui::DragFloat("Jump Height", &jh, 0.1f);
  _jumpHeight = jh;
  ImGui::Text("Jump Key: %s", SDL_GetKeyName(_jumpKey));
  Input::Axis *ah = nullptr;
  for (Input::Axis *a : FindChildrenOfType<Input::Axis>())
  {
    if (!ah && a->Name() == "Axis-Horizontal")
    {
      ah = a;
      break;
    }
  }
  if (ah)
  {
    double dx = ah->GetValue() * _acceleration;
    ImGui::Text("Input X: %.3f", dx);
  }
  ImGui::Text("Jumping: %.3f / %.3f", _jumpRemaining, _jumpHeight);
  Object::PopulateDebugger();
}

} // namespace Controller
} // namespace Aspen
