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
  static float gs;
  gs = _gravStrength;
  static float gd;
  gd = _gravDirection;
  static float d;
  d = _drag;
  ImGui::DragFloat("Gravity Strength", &gs);
  _gravStrength = gs;
  ImGui::SliderFloat("Gravity Direction", &gd, 0.0f, float(M_2_PI));
  _gravDirection = gd;
  ImGui::SliderFloat("Drag", &d, 0.0f, 1.0f);
  _drag = d;
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
  static float m;
  m = _mass;
  static float vs;
  vs = _velocityStrength;
  static float vd;
  vd = _velocityDirection;
  ImGui::DragFloat("Mass", &m, 0.1f, 0.1f, 10000.0f);
  _mass = m;
  ImGui::DragFloat("Velocity Strength", &vs, 0.1f);
  _velocityStrength = vs;
  ImGui::SliderFloat("Velocity Direction", &vd, 0.0f, float(2 * M_PI));
  _velocityDirection = vd;
  ImGui::Text("Cartesian Velocity: (%.4f, %.4f)", GetVelocityX(), GetVelocityY());
  Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();
  if (engine)
  {
    Physics *physics = engine->FindChildOfType<Physics>();
    if (physics)
      ImGui::Text("VDrag: %.4f", _velocityStrength * physics->GetDrag());
  }
  static float as;
  as = _accelerationStrength;
  static float ad;
  ad = _accelerationDirection;
  ImGui::DragFloat("Acceleration Strength", &as, 0.1f);
  _accelerationStrength = as;
  ImGui::SliderFloat("Acceleration Direction", &ad, 0.0f, float(2 * M_2_PI));
  _accelerationDirection = ad;
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
  ImGui::DragInt2("Offset", &_offX, 1.0f);
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
      c.second.collisionAngle = ca + M_PI;
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
  Transform::Transform *tf = FindChildOfType<Transform::Transform>();
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
    double rx = _radius * std::cos(collision.collisionAngle);
    double ry = _radius * std::sin(collision.collisionAngle);
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
  static float r;
  r = _radius;
  ImGui::DragFloat("Radius", &r, 0.5f);
  _radius = r;
  Collider::PopulateDebugger();
}

/////////////////////////////////////////////////////////

AABBCollider::AABBCollider(Object *parent, std::string name)
    : AABBCollider(1, 1, parent, name)
{
}

AABBCollider::AABBCollider(double width, double height, Object *parent, std::string name)
    : Collider(parent, name), _width(width), _height(height)
{
}

std::pair<Collision, Collision> AABBCollider::TestCollision(Collider *other)
{
  std::pair<Collision, Collision> c(Collision(other), Collision(this));
  c.first.result = COLLISION_RESULT::CANNOT_HANDLE;
  c.second.result = COLLISION_RESULT::CANNOT_HANDLE;

  if (dynamic_cast<AABBCollider *>(other))
  {
    Transform::Transform *ttf = FindChildOfType<Transform::Transform>();
    if (!ttf && Parent())
    {
      ttf = Parent()->FindChildOfType<Transform::Transform>();
      if (!ttf)
      {
        c.first.result = COLLISION_RESULT::FAILURE;
        c.second.result = COLLISION_RESULT::FAILURE;
        return c;
      }
    }
    Transform::Transform *otf = other->FindChildOfType<Transform::Transform>();
    if (!otf && other->Parent())
    {
      otf = other->Parent()->FindChildOfType<Transform::Transform>();
      if (!otf)
      {
        c.first.result = COLLISION_RESULT::FAILURE;
        c.second.result = COLLISION_RESULT::FAILURE;
        return c;
      }
    }
    AABBCollider *oc = dynamic_cast<AABBCollider *>(other);
    // this's bounds
    double tl = ttf->GetXPosition() + GetOffsetX() - GetWidth() / 2.0f,
           tr = ttf->GetXPosition() + GetOffsetX() + GetWidth() / 2.0f,
           tt = ttf->GetYPosition() + GetOffsetY() - GetHeight() / 2.0f,
           tb = ttf->GetYPosition() + GetOffsetY() + GetHeight() / 2.0f;
    // other's bounds
    double ol = otf->GetXPosition() + oc->GetOffsetX() - oc->GetWidth() / 2.0f,
           oR = otf->GetXPosition() + oc->GetOffsetX() + oc->GetWidth() / 2.0f,
           ot = otf->GetYPosition() + oc->GetOffsetY() - oc->GetHeight() / 2.0f,
           ob = otf->GetYPosition() + oc->GetOffsetY() + oc->GetHeight() / 2.0f;

    if (tr >= ol && tl <= oR &&
        tb >= ot && tt <= ob)
    {
      c.first.result = COLLISION_RESULT::SUCCESS;
      c.second.result = COLLISION_RESULT::SUCCESS;
      double tcx = (tl + tr) / 2.0,
             tcy = (tb + tt) / 2.0;
      double ocx = (ol + oR) / 2.0,
             ocy = (ob + ot) / 2.0;
      double dx = ocx - tcx,
             dy = ocy - tcy;
      double s;
      if (dx == 0)
        s = 0;
      else
        s = dy / dx;
      double ca = std::atan(s);
      bool h = std::abs(std::cos(ca) / std::max(GetWidth(), oc->GetWidth())) - std::abs(std::sin(ca) / std::max(GetHeight(), oc->GetHeight())) > 0;

      if (h)
      {
        c.first.collisionX = (std::abs(dx) - oc->GetWidth() / 2.0f) * (dx > 0 ? 1 : -1);
        c.first.collisionY = c.first.collisionX * s;
        c.second.collisionX = (std::abs(dx) - GetWidth() / 2.0f) * (dx > 0 ? -1 : 1);
        c.second.collisionY = c.second.collisionX * s;
        c.first.collisionAngle = c.first.collisionX > 0 ? 0 : M_PI;
      }
      else
      {
        c.first.collisionY = (std::abs(dy) - oc->GetHeight() / 2.0f) * (dy > 0 ? 1 : -1);
        c.first.collisionX = c.first.collisionY / s;
        c.second.collisionY = (std::abs(dy) - GetHeight() / 2.0f) * (dy > 0 ? -1 : 1);
        c.second.collisionX = c.second.collisionY / s;
        c.first.collisionAngle = (c.first.collisionY > 0 ? 0 : M_PI) + M_PI / 2.0;
      }
      c.second.collisionAngle = c.first.collisionAngle + M_PI;
    }
    else
    {
      c.first.result = COLLISION_RESULT::FAILURE;
      c.second.result = COLLISION_RESULT::FAILURE;
    }
  }

  return c;
}

void AABBCollider::ResolveCollision(Collision collision)
{
  if (!collision.collider ||
      collision.result != COLLISION_RESULT::SUCCESS ||
      _trigger ||
      collision.collider->IsTrigger())
    return;
  Rigidbody *rb = nullptr;
  Transform::Transform *tf = FindChildOfType<Transform::Transform>();
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
    if (orb)
    {
      tf->ModifyPosition((std::abs(collision.collisionX) - (GetWidth() / 2.0f)) / 2.0f * std::cos(collision.collisionAngle),
                         (std::abs(collision.collisionY) - (GetHeight() / 2.0f)) / 2.0f * std::sin(collision.collisionAngle));
    }
    else
    {
      tf->ModifyPosition((std::abs(collision.collisionX) - (GetWidth() / 2.0f)) * std::cos(collision.collisionAngle),
                         (std::abs(collision.collisionY) - (GetHeight() / 2.0f)) * std::sin(collision.collisionAngle));
    }
  }
}

double AABBCollider::GetWidth()
{
  return _width;
}

void AABBCollider::SetWidth(double width)
{
  _width = width;
}

double AABBCollider::GetHeight()
{
  return _height;
}

void AABBCollider::SetHeight(double height)
{
  _height = height;
}

void AABBCollider::SetSize(double width, double height)
{
  _width = width;
  _height = height;
}

void AABBCollider::PopulateDebugger()
{
  static float s[2];
  s[0] = _width;
  s[1] = _height;
  ImGui::DragFloat2("Size", s, 0.5f, 1.0f, 999999.0f);
  _width = s[0];
  _height = s[1];
  Collider::PopulateDebugger();
}
} // namespace Physics
} // namespace Aspen
