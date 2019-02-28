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
  const double RIGHT = 0;
  const double UP = M_PI_2;
  const double LEFT = M_PI;
  const double DOWN = 3 * M_PI_2;
};

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
  void SetVelocity(double x, double y);

  double GetAccelerationStrength();
  double GetAccelerationDirection();
  double GetAccelerationX();
  double GetAccelerationY();
  void SetAccelerationStrength(double strength);
  void SetAccelerationDirection(double direction);
  void SetAcceleration(double x, double y);
  void SetCartesianAcceleration(double strength, double direction);

  void ApplyForce(double force, double angle);
  void ApplyCartesianForce(double x, double y);
};
} // namespace Physics
} // namespace Aspen

#endif
