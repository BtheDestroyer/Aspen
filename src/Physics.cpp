#define __PHYSICS_CPP

#include "Physics.hpp"
#include "Engine.hpp"
#include "Transform.hpp"
#include "Collision.hpp"
#include "Graphics.hpp"
#include "Time.hpp"
#include "Input.hpp"
#include "Debug.hpp"
#include "Log.hpp"
#include "imgui.h"
#include <limits>

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

  Engine::Engine *engine = Engine::Engine::Get();
  if (engine)
  {
    std::vector<Collider *> colliders = engine->FindDescendentsOfType<Collider>();
    for (unsigned i = 0; i < colliders.size(); ++i)
      if (!colliders[i]->Active())
        colliders.erase(colliders.begin() + i--);
    for (unsigned i = 0; i < colliders.size(); ++i)
    {
      for (unsigned j = i + 1; j < colliders.size(); ++j)
      {
        if (colliders[i]->HasAncestor(colliders[j]->Parent()))
          continue;
        std::pair<Collision, Collision> c = colliders[i]->TestCollision(colliders[j]);
        if (c.first.result == COLLISION_RESULT::SUCCESS)
        {
          colliders[i]->Parent()->OnCollision(c.first);
          colliders[j]->Parent()->OnCollision(c.second);
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
    : Object(parent, name), _mass(mass), _velocityStrength(0), _velocityDirection(0), _accelerationStrength(0), _accelerationDirection(0), _gravityScale(1)
{
}

Rigidbody::~Rigidbody()
{
}

void Rigidbody::operator()()
{
  if (_parent)
  {
    Engine::Engine *engine = Engine::Engine::Get();
    if (engine)
    {
      Physics *physics = engine->FindChildOfType<Physics>();
      if (physics)
      {
        Time::Time *time = Time::Time::Get();
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
        double ax = (GetAccelerationX() + physics->GetGravityX() * _gravityScale - vx * physics->GetDrag()) * dt;
        double ay = (GetAccelerationY() + physics->GetGravityY() * _gravityScale - vy * physics->GetDrag()) * dt;
        SetCartesianVelocity(vx + ax, vy + ay);

        Transform::Transform *tf = _parent->GetTransform();
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

double Rigidbody::GetGravityScale()
{
  return _gravityScale;
}

void Rigidbody::SetGravityScale(double scale)
{
  _gravityScale = scale;
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
  Engine::Engine *engine = Engine::Engine::Get();
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

Collider::Collider(Object *parent, std::string name)
    : Object(parent, name), _trigger(false)
{
  CreateChild<Transform::Transform>();
}

void Collider::operator()()
{
  if (Parent())
  {
    Input::Mouse &m = Input::GetMouse();
    if (InCollider(m.x, m.y))
    {
      if (!_mouseOver)
      {
        _mouseOver = true;
        Parent()->OnMouseEnter();
      }
      Parent()->OnMouseOver();
      if (m.left.pressed | m.middle.pressed | m.right.pressed)
      {
        Parent()->OnMouseClick();
        if (m.middle.pressed)
        {
          Engine::Engine *e = Engine::Engine::Get();
          if (e)
          {
            Debug::Debug *d = e->FindChildOfType<Debug::Debug>();
            if (!d)
            {
              Graphics::Graphics *g = e->FindChildOfType<Graphics::Graphics>();
              if (g)
                d = g->FindChildOfType<Debug::Debug>();
            }
            if (d)
            {
              d->CloseAll();
              d->Open(Parent());
            }
          }
        }
      }
    }
    else if (_mouseOver)
    {
      _mouseOver = false;
      Parent()->OnMouseExit();
    }

    if (m.left.released | m.middle.released | m.right.released)
      if (InCollider(m.x, m.y))
        Parent()->OnMouseRelease();
  }
  Object::operator()();
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

bool Collider::InCollider(int x, int y)
{
  Transform::Transform *tf = GetTransform();
  if (!tf)
  {
    if (!Parent())
      return false;
    tf = Parent()->GetTransform();
    if (!tf)
      return false;
  }
  return x == tf->GetXPosition() && y == tf->GetYPosition();
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
  ImGui::Checkbox("Trigger", &_trigger);
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
    Transform::Transform *ttf = GetTransform();
    if (!ttf)
    {
      if (Parent())
        ttf = Parent()->GetTransform();
      if (!ttf)
      {
        c.first.result = COLLISION_RESULT::FAILURE;
        c.second.result = COLLISION_RESULT::FAILURE;
        return c;
      }
    }
    Transform::Transform *otf = GetTransform();
    if (!otf)
    {
      if (Parent())
        otf = Parent()->GetTransform();
      if (!otf)
      {
        c.first.result = COLLISION_RESULT::FAILURE;
        c.second.result = COLLISION_RESULT::FAILURE;
        return c;
      }
    }
    double ox = otf->GetXPosition();
    double oy = otf->GetYPosition();
    double tx = ttf->GetXPosition();
    double ty = ttf->GetYPosition();
    double dx = ox - tx;
    double dy = oy - ty;
    double d2 = dx * dx + dy * dy;
    double oR = dynamic_cast<CircleCollider *>(other)->GetRadius() * (otf->GetXScale() + otf->GetYScale()) * 0.5f;
    double r = _radius * (ttf->GetXScale() + ttf->GetYScale()) * 0.5f + oR;
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
  else if (dynamic_cast<AABBCollider *>(other))
  {
    std::pair<Collision, Collision> c2 = dynamic_cast<AABBCollider *>(other)->TestCollision(this);
    c.first = c2.second;
    c.second = c2.first;
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
  Rigidbody *rb = GetRigidbody();
  Transform::Transform *tf = GetTransform();
  if (!rb)
  {
    if (!_parent)
      return;
    if (!rb)
      rb = Parent()->GetRigidbody();
    if (!rb)
      return;
    tf = Parent()->GetTransform();
    if (!tf)
    {
      GetTransform();
      if (!tf)
        return;
    }
  }

  Rigidbody *orb = collision.collider->GetRigidbody();
  if (!orb)
    orb = collision.collider->Parent()->GetRigidbody();

  double rx = _radius * std::cos(collision.collisionAngle);
  double ry = _radius * std::sin(collision.collisionAngle);
  double dx = rx - collision.collisionX;
  double dy = ry - collision.collisionY;
  if (orb)
  {
    tf->ModifyPosition(-dx / 2, -dy / 2);
  }
  else
  {
    tf->ModifyPosition(-dx, -dy);
    rb->SetCartesianVelocity(rb->GetVelocityX() * std::sin(collision.collisionAngle), rb->GetVelocityY() * std::cos(collision.collisionAngle));
  }
}

bool CircleCollider::InCollider(int x, int y)
{
  Transform::Transform *tf = GetTransform();
  if (!tf)
  {
    if (!Parent())
      return false;
    tf = Parent()->GetTransform();
    if (!tf)
      return false;
  }
  Engine::Engine *engine = Engine::Engine::Get();
  Transform::Transform *ctf = nullptr;
  if (engine)
  {
    Graphics::Graphics *gfx = Graphics::Graphics::Get();
    if (gfx)
    {
      Graphics::Camera *cam = gfx->GetCamera();
      if (cam)
        ctf = cam->GetTransform();
    }
  }
  if (ctf)
  {
    x -= tf->GetXPosition(ctf);
    y -= tf->GetYPosition(ctf);
  }
  else
  {
    x -= tf->GetXPosition();
    y -= tf->GetYPosition();
  }
  double d2 = x * x + y * y;
  double r2 = GetRadius();
  r2 *= r2;
  return d2 <= r2;
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
    Transform::Transform *ttf = GetTransform();
    if (!ttf && Parent())
    {
      ttf = Parent()->GetTransform();
      if (!ttf)
      {
        c.first.result = COLLISION_RESULT::FAILURE;
        c.second.result = COLLISION_RESULT::FAILURE;
        return c;
      }
    }
    Transform::Transform *otf = other->GetTransform();
    if (!otf && other->Parent())
    {
      otf = other->Parent()->GetTransform();
      if (!otf)
      {
        c.first.result = COLLISION_RESULT::FAILURE;
        c.second.result = COLLISION_RESULT::FAILURE;
        return c;
      }
    }
    AABBCollider *oc = dynamic_cast<AABBCollider *>(other);
    // this's bounds
    double tl = ttf->GetXPosition() - GetWidth() * ttf->GetXScale() / 2.0f,
           tr = ttf->GetXPosition() + GetWidth() * ttf->GetXScale() / 2.0f,
           tt = ttf->GetYPosition() - GetHeight() * ttf->GetYScale() / 2.0f,
           tb = ttf->GetYPosition() + GetHeight() * ttf->GetYScale() / 2.0f;
    // other's bounds
    double ol = otf->GetXPosition() - oc->GetWidth() * otf->GetXScale() / 2.0f,
           oR = otf->GetXPosition() + oc->GetWidth() * otf->GetXScale() / 2.0f,
           ot = otf->GetYPosition() - oc->GetHeight() * otf->GetYScale() / 2.0f,
           ob = otf->GetYPosition() + oc->GetHeight() * otf->GetYScale() / 2.0f;

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
      double ca;
      if (dx == 0)
      {
        s = std::numeric_limits<double>::max();
        ca = M_PI_2;
      }
      else
      {
        s = dy / dx;
        ca = std::atan(s);
      }
      bool h = std::abs(std::cos(ca) / std::max(GetWidth() * ttf->GetXScale(), oc->GetWidth() * otf->GetXScale())) - std::abs(std::sin(ca) / std::max(GetHeight() * ttf->GetYScale(), oc->GetHeight() * otf->GetYScale())) > 0;

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
        c.first.collisionAngle = (c.first.collisionY > 0 ? 0 : M_PI) + M_PI_2;
      }
      c.second.collisionAngle = c.first.collisionAngle + M_PI;
    }
    else
    {
      c.first.result = COLLISION_RESULT::FAILURE;
      c.second.result = COLLISION_RESULT::FAILURE;
    }
  }
  else if (dynamic_cast<CircleCollider *>(other))
  {
    Transform::Transform *ttf = GetTransform();
    if (!ttf && Parent())
    {
      ttf = Parent()->GetTransform();
      if (!ttf)
      {
        c.first.result = COLLISION_RESULT::FAILURE;
        c.second.result = COLLISION_RESULT::FAILURE;
        return c;
      }
    }
    Transform::Transform *otf = other->GetTransform();
    if (!otf && other->Parent())
    {
      otf = other->Parent()->GetTransform();
      if (!otf)
      {
        c.first.result = COLLISION_RESULT::FAILURE;
        c.second.result = COLLISION_RESULT::FAILURE;
        return c;
      }
    }
    CircleCollider *oc = dynamic_cast<CircleCollider *>(other);
    double tx = ttf->GetXPosition(),
           ty = ttf->GetYPosition();
    double ox = otf->GetXPosition(),
           oy = otf->GetYPosition();
    double dx = ox - tx,
           dy = oy - ty;
    double d2 = dx * dx + dy * dy,
           d = std::sqrt(d2);
    double maxd = std::sqrt(GetWidth() * ttf->GetXScale() / 2 * GetWidth() * ttf->GetXScale() / 2 + GetHeight() * ttf->GetYScale() / 2 * GetHeight() * ttf->GetYScale() / 2) + oc->GetRadius() * (otf->GetXScale() + otf->GetYScale()) * 0.5f;
    // broad check
    if (d > maxd)
    {
      c.first.result = COLLISION_RESULT::FAILURE;
      c.second.result = COLLISION_RESULT::FAILURE;
      return c;
    }
    double s;
    if (dx == 0)
      s = 0;
    else
      s = dy / dx;
    double ca = std::atan2(dy, dx);
    double odx = cos(ca) * oc->GetRadius(),
           ody = sin(ca) * oc->GetRadius();
    double tdx = (dx - odx),
           tdy = (dy - ody);
    if (std::abs(tdx) >= GetWidth() / 2 || std::abs(tdy) >= GetHeight() / 2)
    {
      c.first.result = COLLISION_RESULT::FAILURE;
      c.second.result = COLLISION_RESULT::FAILURE;
      return c;
    }
    bool h = std::abs(std::cos(ca) / GetWidth() * ttf->GetXScale()) - std::abs(std::sin(ca) / GetHeight() * ttf->GetYScale()) > 0;
    double td = std::sqrt(tdx * tdx + tdy * tdy);
    if (h)
    {
      odx = (std::abs(dx) - GetWidth() * ttf->GetXScale() / 2.0f);
      ody = td * s;
    }
    else
    {
      ody = (std::abs(dy) - GetHeight() * ttf->GetYScale() / 2.0f);
      odx = td / s;
    }
    c.first.collisionX = tdx;
    c.first.collisionY = tdy;
    c.first.collisionAngle = (h ? (c.first.collisionX > 0 ? 0 : M_PI) : (c.first.collisionY > 0 ? 0 : M_PI) + M_PI / 2.0);
    c.second.collisionAngle = c.first.collisionAngle + M_PI;
    c.second.collisionX = odx * std::cos(c.second.collisionAngle);
    c.second.collisionY = ody * std::sin(c.second.collisionAngle);
    c.first.result = COLLISION_RESULT::SUCCESS;
    c.second.result = COLLISION_RESULT::SUCCESS;
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
  Rigidbody *rb = GetRigidbody();
  Transform::Transform *tf = GetTransform();
  if (!rb)
  {
    if (!_parent)
      return;
    if (!rb)
      rb = Parent()->GetRigidbody();
    if (!rb)
      return;
    tf = Parent()->GetTransform();
    if (!tf)
    {
      GetTransform();
      if (!tf)
        return;
    }
  }

  Rigidbody *orb = collision.collider->GetRigidbody();
  if (!orb)
    orb = collision.collider->Parent()->GetRigidbody();

  if (orb)
  {
    tf->ModifyPosition((std::abs(collision.collisionX) - (GetWidth() / 2.0f)) / 2.0f * std::cos(collision.collisionAngle),
                       (std::abs(collision.collisionY) - (GetHeight() / 2.0f)) / 2.0f * std::sin(collision.collisionAngle));
  }
  else
  {
    tf->ModifyPosition((std::abs(collision.collisionX) - (GetWidth() / 2.0f)) * std::cos(collision.collisionAngle),
                       (std::abs(collision.collisionY) - (GetHeight() / 2.0f)) * std::sin(collision.collisionAngle));
    rb->SetCartesianVelocity(rb->GetVelocityX() * std::sin(collision.collisionAngle), rb->GetVelocityY() * std::cos(collision.collisionAngle));
  }
}

bool AABBCollider::InCollider(int x, int y)
{
  Transform::Transform *tf = GetTransform();
  if (!tf)
  {
    if (!Parent())
      return false;
    tf = Parent()->GetTransform();
    if (!tf)
      return false;
  }
  Engine::Engine *engine = Engine::Engine::Get();
  Transform::Transform *ctf = nullptr;
  if (engine)
  {
    Graphics::Graphics *gfx = Graphics::Graphics::Get();
    if (gfx)
    {
      Graphics::Camera *cam = gfx->GetCamera();
      if (cam)
        ctf = cam->GetTransform();
    }
  }
  if (ctf)
  {
    x -= tf->GetXPosition(ctf);
    y -= tf->GetYPosition(ctf);
  }
  else
  {
    x -= tf->GetXPosition();
    y -= tf->GetYPosition();
  }
  x = std::abs(x) * 2;
  y = std::abs(y) * 2;
  return x <= GetWidth() && y <= GetHeight();
}

double AABBCollider::GetWidth()
{
  if (GetTransform())
    return _width * GetTransform()->GetLocalXScale();
  return _width;
}

void AABBCollider::SetWidth(double width)
{
  _width = width;
}

double AABBCollider::GetHeight()
{
  if (GetTransform())
    return _height * GetTransform()->GetLocalYScale();
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
