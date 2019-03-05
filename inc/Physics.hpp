#ifndef __PHYSICS_HPP
#define __PHYSICS_HPP
#include "Object.hpp"
#include <string>
#include <cmath>

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Physics namespace
namespace Physics
{
namespace GRAV_DIR
{
const double LEFT = 0;
const double DOWN = M_PI_2;
const double RIGHT = M_PI;
const double UP = 3 * M_PI_2;
}; // namespace GRAV_DIR

class Physics : public Object::Object
{
  double _gravStrength;
  double _gravDirection;
  double _drag;

public:
  Physics(Object *parent = nullptr, std::string name = "Physics");
  Physics(double strength, double direction = GRAV_DIR::DOWN, Object *parent = nullptr, std::string name = "Physics");
  Physics(double strength, double direction, double drag, Object *parent = nullptr, std::string name = "Physics");
  ~Physics();

  void operator()();

  double GetGravityStrength();
  double GetGravityDirection();
  double GetGravityX();
  double GetGravityY();
  void SetGravityStrength(double strength);
  void SetGravityDirection(double direction);
  void SetGravity(double x, double y);

  double GetDrag();
  void SetDrag(double drag);

  void PopulateDebugger();
};

class Rigidbody : public Object::Object
{
  double _mass;
  double _velocityStrength;
  double _velocityDirection;
  double _accelerationStrength;
  double _accelerationDirection;

public:
  Rigidbody(Object *parent = nullptr, std::string name = "Rigidbody");
  Rigidbody(double mass, Object *parent = nullptr, std::string name = "Rigidbody");
  ~Rigidbody();

  void operator()();

  double GetMass();
  void SetMass(double mass);

  double GetVelocityStrength();
  double GetVelocityDirection();
  double GetVelocityX();
  double GetVelocityY();
  void SetVelocityStrength(double strength);
  void SetVelocityDirection(double direction);
  void SetVelocity(double strength, double direction);
  void SetCartesianVelocity(double x, double y);

  double GetAccelerationStrength();
  double GetAccelerationDirection();
  double GetAccelerationX();
  double GetAccelerationY();
  void SetAccelerationStrength(double strength);
  void SetAccelerationDirection(double direction);
  void SetAcceleration(double strength, double direction);
  void SetCartesianAcceleration(double x, double y);

  void ApplyForce(double force, double angle);
  void ApplyCartesianForce(double x, double y);

  void PopulateDebugger();
};

/////////////////////////////////////////////////////////

enum COLLISION_RESULT
{
  FAILURE,
  SUCCESS,
  CANNOT_HANDLE
};

class Collider;

class Collision
{
public:
  COLLISION_RESULT result;
  Collider *collider;
  double collisionX;
  double collisionY;
  double forceDirection;
  double forceStrength;

  Collision(Collider *other);
};

class Collider : public Object::Object
{
protected:
  int _offX;
  int _offY;
  bool _trigger;

public:
  Collider(Object *parent = nullptr, std::string name = "Collider");

  void operator()();

  virtual Collision TestCollision(Collider *other);
  virtual void ResolveCollision(Collision collision);

  void SetOffset(int x, int y);
  int GetOffsetX();
  int GetOffsetY();

  int GetX();
  int GetY();

  bool IsTrigger();
  void SetTrigger(bool trigger);

  void PopulateDebugger();
};

class CircleCollider : public Collider
{
  double _radius;

public:
  CircleCollider(Object *parent = nullptr, std::string name = "CircleCollider");
  CircleCollider(double radius, Object *parent = nullptr, std::string name = "CircleCollider");

  Collision TestCollision(Collider *other);
  void ResolveCollision(Collision collision);

  double GetRadius();
  void SetRadius(double radius);

  void PopulateDebugger();
};
} // namespace Physics
} // namespace Aspen

#endif
