#define __PHYSICS_CPP

#include "Physics.hpp"
#include "Engine.hpp"
#include "Transform.hpp"
#include "Time.hpp"
#include "Log.hpp"

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

/////////////////////////////////////////////////////////

Rigidbody::Rigidbody(Object *parent, std::string name)
    : Rigidbody(1, parent, name)
{
}

Rigidbody::Rigidbody(double mass, Object *parent, std::string name)
    : Object(parent, name), _mass(mass)
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

        SetVelocity(GetVelocityX() + GetAccelerationX() * dt, GetVelocityY() + GetAccelerationY() * dt);
        _accelerationStrength *= physics->GetDrag() * dt;

        Transform::Transform *tf = _parent->FindChildOfType<Transform::Transform>();
        if (tf)
          tf->ModifyPosition(GetVelocityX(), GetVelocityY());
        else
          Log::Warning("Rigidbody requires a parent with a Transform child!");
      }
      else
        Log::Warning("Rigidbody requires an ancestor Engine with a Physics child!");
    }
    else
      Log::Warning("Rigidbody requires an ancestor Engine with a Physics child!");
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

void Rigidbody::SetAcceleration(double x, double y)
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
} // namespace Physics
} // namespace Aspen
