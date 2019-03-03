#define __PHYSICS_CPP

#include "Physics.hpp"
#include "Engine.hpp"
#include "Transform.hpp"
#include "Time.hpp"
#include "Log.hpp"
#include "imgui.h"

#undef __PHYSICS_CPP

namespace Aspen
{
namespace Physics
{
Physics::Physics(Object *parent, std::string name)
    : Physics(1, GRAV_DIR::DOWN, parent, name)
{
}

Physics::Physics(double strength, double direction, Object *parent, std::string name)
    : Object(parent, name), _gravStrength(strength), _gravDirection(direction)
{
}

Physics::~Physics()
{
}

void Physics::operator()()
{
  Object::operator()();
}

double Physics::GetGravityStrength()
{
  return _gravStrength;
}

double Physics::GetGravityDirection()
{
  return _gravDirection;
}

double Physics::GetGravityX()
{
  return _gravStrength * std::cos(_gravDirection);
}

double Physics::GetGravityY()
{
  return _gravStrength * std::sin(_gravDirection);
}

void Physics::SetGravityStrength(double strength)
{
  _gravStrength = strength;
}

void Physics::SetGravityDirection(double direction)
{
  _gravDirection = direction;
}

void Physics::SetGravity(double x, double y)
{
  _gravDirection = std::atan2(x, y);
  _gravStrength = std::sqrt(x * x + y * y);
}

double Physics::GetDrag()
{
  return _drag;
}

void Physics::SetDrag(double drag)
{
  _drag = drag;
}

void Physics::PopulateDebugger()
{
  ImGui::Text("Gravity: %.4f, %.4f", _gravStrength, _gravDirection);
  ImGui::Text("Drag: %f", _drag);
  Object::PopulateDebugger();
}

/////////////////////////////////////////////////////////

Rigidbody::Rigidbody(Object *parent, std::string name)
    : Rigidbody(1, parent, name)
{
}

Rigidbody::Rigidbody(double mass, Object *parent, std::string name)
    : Object(parent, name), _mass(mass), _velocityStrength(0), _velocityDirection(0), _accelerationStrength(0), _accelerationDirection(0)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::operator()()
{
  if (_parent)
  {
    Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();
    if (engine)
    {
      Physics *physics = engine->FindChildOfType<Physics>();
      if (physics)
      {
        Time::Time *time = engine->FindChildOfType<Time::Time>();
        double dt;
        if (time)
          dt = time->DeltaTime() * 60.0;
        else
          dt = 1.0;

        _velocityStrength *= physics->GetDrag() * dt;
        SetVelocity(GetVelocityX() + GetAccelerationX() * dt, GetVelocityY() + GetAccelerationY() * dt);
        SetCartesianAcceleration(GetAccelerationX() + physics->GetGravityX(), GetAccelerationY() + physics->GetGravityY());

        Transform::Transform *tf = _parent->FindChildOfType<Transform::Transform>();
        if (tf)
          tf->ModifyPosition(GetVelocityX(), GetVelocityY());
        else
          Log::Warning("%s requires a parent with a Transform child!", Name().c_str());
      }
      else
        Log::Error("%s requires an ancestor Engine with a Physics child!", Name().c_str());
    }
    else
      Log::Error("%s requires an ancestor Engine with a Physics child!", Name().c_str());
  }

  Object::operator()();
}

double Rigidbody::GetMass()
{
  return _mass;
}

void Rigidbody::SetMass(double mass)
{
  _mass = mass;
}

double Rigidbody::GetVelocityStrength()
{
  return _velocityStrength;
}

double Rigidbody::GetVelocityDirection()
{
  return _velocityDirection;
}

double Rigidbody::GetVelocityX()
{
  return _velocityStrength * std::cos(_velocityDirection);
}

double Rigidbody::GetVelocityY()
{
  return _velocityStrength * std::sin(_velocityDirection);
}

void Rigidbody::SetVelocityStrength(double strength)
{
  _velocityStrength = strength;
}

void Rigidbody::SetVelocityDirection(double direction)
{
  _velocityDirection = direction;
}

void Rigidbody::SetVelocity(double x, double y)
{
  _velocityDirection = std::atan2(x, y);
  _velocityStrength = std::sqrt(x * x + y * y);
}

double Rigidbody::GetAccelerationStrength()
{
  return _accelerationStrength;
}

double Rigidbody::GetAccelerationDirection()
{
  return _accelerationDirection;
}

double Rigidbody::GetAccelerationX()
{
  return _accelerationStrength * std::cos(_accelerationDirection);
}

double Rigidbody::GetAccelerationY()
{
  return _accelerationStrength * std::sin(_accelerationDirection);
}

void Rigidbody::SetAccelerationStrength(double strength)
{
  _accelerationStrength = strength;
}

void Rigidbody::SetAccelerationDirection(double direction)
{
  _accelerationDirection = direction;
}

void Rigidbody::SetAcceleration(double strength, double direction)
{
  _accelerationDirection = direction;
  _accelerationStrength = strength;
}

void Rigidbody::SetCartesianAcceleration(double x, double y)
{
  _accelerationDirection = std::atan2(x, y);
  _accelerationStrength = std::sqrt(x * x + y * y);
}

void Rigidbody::ApplyForce(double force, double angle)
{
  force /= _mass;
  double fx = std::cos(angle) * force;
  double fy = std::sin(angle) * force;
  SetAcceleration(GetAccelerationX() + fx, GetAccelerationY() + fy);
}

void Rigidbody::ApplyCartesianForce(double x, double y)
{
  SetAcceleration(GetAccelerationX() + x / _mass, GetAccelerationY() + y / _mass);
}

void Rigidbody::PopulateDebugger()
{
  ImGui::Text("Mass: %f", _mass);
  ImGui::Text("Velocity: %.4f, %.4f", _velocityStrength, _velocityDirection);
  Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();
  if (engine)
  {
    Physics *physics = engine->FindChildOfType<Physics>();
    if (physics)
      ImGui::Text("VDrag: %.4f", _velocityStrength * physics->GetDrag());
  }
  ImGui::Text("Acceleration: %.4f, %.4f", _accelerationStrength, _accelerationDirection);
  Object::PopulateDebugger();
}
} // namespace Physics
} // namespace Aspen
