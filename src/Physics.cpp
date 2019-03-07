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

  Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();
  if (engine)
  {
    std::vector<Collider *> colliders = engine->FindDescendentsOfType<Collider>();
    for (unsigned i = 0; i < colliders.size(); ++i)
    {
      for (unsigned j = i + 1; j < colliders.size(); ++j)
      {
        std::pair<Collision, Collision> c = colliders[i]->TestCollision(colliders[j]);
        if (c.first.result == COLLISION_RESULT::SUCCESS)
        {
          colliders[i]->ResolveCollision(c.first);
          colliders[j]->ResolveCollision(c.second);
        }
        else if (c.first.result == COLLISION_RESULT::CANNOT_HANDLE)
        {
          c = colliders[j]->TestCollision(colliders[i]);
          if (c.first.result == COLLISION_RESULT::SUCCESS)
          {
            colliders[j]->ResolveCollision(c.first);
            colliders[i]->ResolveCollision(c.second);
          }
        }
      }
    }
  }
  else
    Log::Error("%s must have an anscestor of type Engine::Engine!", Name().c_str());
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
  _gravDirection = std::atan2(y, x);
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

        //_velocityStrength *= physics->GetDrag() * dt;
        //SetCartesianVelocity(GetVelocityX() + GetAccelerationX() * dt, GetVelocityY() + GetAccelerationY() * dt);
        //SetCartesianAcceleration(GetAccelerationX() + physics->GetGravityX(), GetAccelerationY() + physics->GetGravityY());
        double vx = GetVelocityX();
        double vy = GetVelocityY();
        vx = std::abs(vx) >= 0.001 ? vx : 0.0;
        vy = std::abs(vy) >= 0.001 ? vy : 0.0;
        double ax = (GetAccelerationX() + physics->GetGravityX() - vx * physics->GetDrag()) * dt;
        double ay = (GetAccelerationY() + physics->GetGravityY() - vy * physics->GetDrag()) * dt;
        SetCartesianVelocity(vx + ax, vy + ay);

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

void Rigidbody::SetVelocity(double strength, double direction)
{
  _velocityStrength = strength;
  _velocityDirection = direction;
}

void Rigidbody::SetCartesianVelocity(double x, double y)
{
  _velocityDirection = std::atan2(y, x);
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
  _accelerationDirection = std::atan2(y, x);
  _accelerationStrength = std::sqrt(x * x + y * y);
}

void Rigidbody::ApplyForce(double force, double angle)
{
  force /= _mass;
  double fx = std::cos(angle) * force;
  double fy = std::sin(angle) * force;
  SetCartesianVelocity(GetVelocityX() + fx, GetVelocityY() + fy);
}

void Rigidbody::ApplyCartesianForce(double x, double y)
{
  SetCartesianVelocity(GetVelocityX() + x / _mass, GetVelocityY() + y / _mass);
}

void Rigidbody::PopulateDebugger()
{
  ImGui::Text("Mass: %f", _mass);
  ImGui::Text("Velocity: %.4f, %.4f (%.4f, %.4f)", _velocityStrength, _velocityDirection, GetVelocityX(), GetVelocityY());
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

/////////////////////////////////////////////////////////

Collision::Collision(Collider *other)
    : collider(other), collisionX(0), collisionY(0), collisionAngle(0)
{
}

/////////////////////////////////////////////////////////

Collider::Collider(Object *parent, std::string name)
    : Object(parent, name), _offX(0), _offY(0), _trigger(false)
{
}

void Collider::operator()()
{
}

std::pair<Collision, Collision> Collider::TestCollision(Collider *other)
{
  std::pair<Collision, Collision> c(Collision(other), Collision(this));
  c.first.result = COLLISION_RESULT::CANNOT_HANDLE;
  c.second.result = COLLISION_RESULT::CANNOT_HANDLE;
  return c;
}

void Collider::ResolveCollision(Collision collision)
{
}

void Collider::SetOffset(int x, int y)
{
  _offX = x;
  _offY = y;
}

int Collider::GetOffsetX()
{
  return _offX;
}

int Collider::GetOffsetY()
{
  return _offY;
}

int Collider::GetX()
{
  Transform::Transform *tf = FindChildOfType<Transform::Transform>();
  if (!tf)
  {
    if (!_parent)
      return _offX;
    tf = _parent->FindChildOfType<Transform::Transform>();
    if (!tf)
      return _offX;
  }
  return _offX + tf->GetXPosition();
}

int Collider::GetY()
{
  Transform::Transform *tf = FindChildOfType<Transform::Transform>();
  if (!tf)
  {
    if (!_parent)
      return _offY;
    tf = _parent->FindChildOfType<Transform::Transform>();
    if (!tf)
      return _offY;
  }
  return _offY + tf->GetYPosition();
}

bool Collider::IsTrigger()
{
  return _trigger;
}

void Collider::SetTrigger(bool trigger)
{
  _trigger = trigger;
}

void Collider::PopulateDebugger()
{
  ImGui::Text("Offset: %d, %d", _offX, _offY);
  Object::PopulateDebugger();
}

/////////////////////////////////////////////////////////

CircleCollider::CircleCollider(Object *parent, std::string name)
    : CircleCollider(1, parent, name)
{
}

CircleCollider::CircleCollider(double radius, Object *parent, std::string name)
    : Collider(parent, name), _radius(radius)
{
}

std::pair<Collision, Collision> CircleCollider::TestCollision(Collider *other)
{
  std::pair<Collision, Collision> c(Collision(other), Collision(this));
  c.first.result = COLLISION_RESULT::CANNOT_HANDLE;
  c.second.result = COLLISION_RESULT::CANNOT_HANDLE;

  if (dynamic_cast<CircleCollider *>(other))
  {
    double ox = other->GetX();
    double oy = other->GetY();
    double tx = GetX();
    double ty = GetY();
    double dx = ox - tx;
    double dy = oy - ty;
    double d2 = dx * dx + dy * dy;
    double oR = dynamic_cast<CircleCollider *>(other)->GetRadius();
    double r = _radius + oR;
    if (d2 < r * r)
    {
      c.first.result = COLLISION_RESULT::SUCCESS;
      c.second.result = COLLISION_RESULT::SUCCESS;
      double d = std::sqrt(d2);
      double cd = d - oR;
      double ocd = d - _radius;
      double ca = std::atan2(dy, dx);
      c.first.collisionX = std::cos(ca) * cd;
      c.first.collisionY = std::sin(ca) * cd;
      c.first.collisionAngle = ca;
      c.second.collisionX = std::cos(M_PI + ca) * ocd;
      c.second.collisionY = std::sin(M_PI + ca) * ocd;
      c.second.collisionAngle = -ca;
    }
    else
    {
      c.first.result = COLLISION_RESULT::FAILURE;
      c.second.result = COLLISION_RESULT::FAILURE;
    }
  }

  return c;
}

void CircleCollider::ResolveCollision(Collision collision)
{
  if (!collision.collider ||
      collision.result != COLLISION_RESULT::SUCCESS ||
      _trigger ||
      collision.collider->IsTrigger())
    return;
  Rigidbody *rb = nullptr;
  Transform::Transform *tf = nullptr;
  tf = FindChildOfType<Transform::Transform>();
  if (!tf)
  {
    if (!_parent)
      return;
    tf = _parent->FindChildOfType<Transform::Transform>();
    if (!tf)
      return;
    rb = _parent->FindChildOfType<Rigidbody>();
  }
  else
    rb = FindChildOfType<Rigidbody>();

  Rigidbody *orb = collision.collider->FindChildOfType<Rigidbody>();
  if (!orb)
    orb = collision.collider->Parent()->FindChildOfType<Rigidbody>();

  if (rb)
  {
    double rx = _radius * std::cos(std::atan2(collision.collisionY, collision.collisionX));
    double ry = _radius * std::sin(std::atan2(collision.collisionY, collision.collisionX));
    double dx = rx - collision.collisionX;
    double dy = ry - collision.collisionY;
    if (orb)
      tf->ModifyPosition(-dx / 2, -dy / 2);
    else
      tf->ModifyPosition(-dx, -dy);
  }
}

double CircleCollider::GetRadius()
{
  return _radius;
}

void CircleCollider::SetRadius(double radius)
{
  _radius = radius;
}

void CircleCollider::PopulateDebugger()
{
  ImGui::Text("Radius: %f", _radius);
  Collider::PopulateDebugger();
}
} // namespace Physics
} // namespace Aspen
