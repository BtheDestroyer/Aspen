#ifndef __PHYSICS_HPP
#define __PHYSICS_HPP
#include "Object.hpp"
#include <string>
#define _USE_MATH_DEFINES
#include <cmath>

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Physics namespace
namespace Physics
{
/// \brief Potential gravity direction defaults
namespace GRAV_DIR
{
/// \brief Left
const double LEFT = 0;
/// \brief Down
const double DOWN = M_PI * 0.5;
/// \brief Right
const double RIGHT = M_PI;
/// \brief Up
const double UP = M_PI * 1.5;
}; // namespace GRAV_DIR

/// \brief Physics class
class Physics : public Object::Object
{
  /// \brief Gravity strength
  double _gravStrength;
  /// \brief Gravity direction
  double _gravDirection;
  /// \brief Drag factor
  double _drag;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Physics(Object *parent = nullptr, std::string name = "Physics");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param strength Gravity strength
  /// \param direction Gravity Direction
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Physics(double strength, double direction = GRAV_DIR::DOWN, Object *parent = nullptr, std::string name = "Physics");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param strength Gravity strength
  /// \param direction Gravity Direction
  /// \param drag Drag factor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Physics(double strength, double direction, double drag, Object *parent = nullptr, std::string name = "Physics");
  /// \brief Destructor
  ~Physics();

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Gets the current gravity strength
  /// \return Gravity strength
  double GetGravityStrength();
  /// \brief Gets the current gravity direction
  /// \return Gravity direction
  double GetGravityDirection();
  /// \brief Gets the current horizontal cartesian gravity strength
  /// \return Horizontal cartesian gravity strength
  double GetGravityX();
  /// \brief Gets the current vertical cartesian gravity strength
  /// \return Vertical cartesian gravity strength
  double GetGravityY();
  /// \brief Sets the gravity strength
  /// \param strength New gravity strength
  void SetGravityStrength(double strength);
  /// \brief Sets the gravity direction
  /// \param direction New gravity direction
  void SetGravityDirection(double direction);
  /// \brief Sets the gravity strength and direction via cartesian coordinates
  /// \param x Horizontal cartesian gravity strength
  /// \param y Vertical cartesian gravity strength
  void SetGravity(double x, double y);

  /// \brief Gets the current drag factor
  /// \return Drag factor
  double GetDrag();
  /// \brief Sets the drag factor
  /// \param drag New drag factor
  void SetDrag(double drag);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief Rigidbody class
class Rigidbody : public Object::Object
{
  /// \brief Mass of the object
  double _mass;
  /// \brief Strength of velocity
  double _velocityStrength;
  /// \brief Direction of velocity
  double _velocityDirection;
  /// \brief Strength of acceleration
  double _accelerationStrength;
  /// \brief Direction of acceleration
  double _accelerationDirection;
  /// \brief Amount gravity affects this object
  double _gravityScale;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Rigidbody(Object *parent = nullptr, std::string name = "Rigidbody");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param mass Mass of the object
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Rigidbody(double mass, Object *parent = nullptr, std::string name = "Rigidbody");
  /// \brief Destructor
  ~Rigidbody();

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Gets the object's mass
  /// \return Object's mass
  double GetMass();
  /// \brief Sets the object's mass
  /// \param mass New mass
  void SetMass(double mass);

  /// \brief Gets the current velocity strength
  /// \return Velocity strength
  double GetVelocityStrength();
  /// \brief Gets the current velocity direction
  /// \return Velocity direction
  double GetVelocityDirection();
  /// \brief Gets the current horizontal cartesian velocity strength
  /// \return Horizontal cartesian velocity strength
  double GetVelocityX();
  /// \brief Gets the current vertical cartesian velocity strength
  /// \return Vertical cartesian velocity strength
  double GetVelocityY();
  /// \brief Sets the velocity strength
  /// \param strength New velocity strength
  void SetVelocityStrength(double strength);
  /// \brief Sets the velocity direction
  /// \param direction New velocity direction
  void SetVelocityDirection(double direction);
  /// \brief Sets the velocity
  /// \param strength New velocity strength
  /// \param direction New velocity direction
  void SetVelocity(double strength, double direction);
  /// \brief Sets the velocity strength and direction via cartesian coordinates
  /// \param x Horizontal cartesian velocity strength
  /// \param y Vertical cartesian velocity strength
  void SetCartesianVelocity(double x, double y);

  /// \brief Gets the current acceleration strength
  /// \return Acceleration strength
  double GetAccelerationStrength();
  /// \brief Gets the current acceleration direction
  /// \return Acceleration direction
  double GetAccelerationDirection();
  /// \brief Gets the current horizontal cartesian acceleration strength
  /// \return Horizontal cartesian acceleration strength
  double GetAccelerationX();
  /// \brief Gets the current vertical cartesian acceleration strength
  /// \return Vertical cartesian acceleration strength
  double GetAccelerationY();
  /// \brief Sets the acceleration strength
  /// \param strength New acceleration strength
  void SetAccelerationStrength(double strength);
  /// \brief Sets the acceleration direction
  /// \param direction New acceleration direction
  void SetAccelerationDirection(double direction);
  /// \brief Sets the acceleration
  /// \param strength New acceleration strength
  /// \param direction New acceleration direction
  void SetAcceleration(double strength, double direction);
  /// \brief Sets the acceleration strength and direction via cartesian coordinates
  /// \param x Horizontal cartesian acceleration strength
  /// \param y Vertical cartesian acceleration strength
  void SetCartesianAcceleration(double x, double y);

  /// \brief Applies a force to the Rigidbody
  /// \param force Strength of the force
  /// \param angle Angle of the force
  void ApplyForce(double force, double angle);
  /// \brief Applies a cartesian force to the Rigidbody
  /// \param x Horizontal strength of the force
  /// \param y Vertical strength of the force
  void ApplyCartesianForce(double x, double y);

  /// \brief Sets the object's gravity scale
  /// \return Object's gravity scale
  double GetGravityScale();
  /// \brief Sets the object's gravity scale
  /// \param mass New gravity scale
  void SetGravityScale(double scale);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/////////////////////////////////////////////////////////

/// \brief Collider class
class Collider : public Object::Object
{
protected:
  /// \brief Determines if the collider is a trigger or solid object
  bool _trigger;
  /// \brief Determines if the collider is being moused over
  bool _mouseOver;

public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Collider(Object *parent = nullptr, std::string name = "Collider");

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Finds if there is a collision between this Collider and another Collider
  /// \param other Second collider
  /// \return Collisions found between the two objects
  ///         CANNOT_HANDLE is used to instead have other test against this Collider
  virtual std::pair<Collision, Collision> TestCollision(Collider *other);
  /// \brief Resolves a collision and passes the collision to the parent via OnCollision
  /// \param collision Collision to resolve
  virtual void ResolveCollision(Collision collision);
  /// \brief Determines if a point is within the collider
  /// \param x World x position
  /// \param y World y position
  /// \return True if (x, y) is within the collider
  ///         False otherwise
  virtual bool InCollider(int x, int y);

  /// \brief Determines if the collider is a trigger or solid object
  /// \return _trigger
  bool IsTrigger();
  /// \brief Determines if the collider is a trigger or solid object
  /// \param trigger New trigger status
  void SetTrigger(bool trigger);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief CircleCollider class
class CircleCollider : public Collider
{
  /// \brief Radius of the collider
  double _radius;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  CircleCollider(Object *parent = nullptr, std::string name = "CircleCollider");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param radius Radius of the collider
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  CircleCollider(double radius, Object *parent = nullptr, std::string name = "CircleCollider");

  /// \brief Finds if there is a collision between this Collider and another Collider
  /// \param other Second collider
  /// \return Collisions found between the two objects
  ///         CANNOT_HANDLE is used to instead have other test against this Collider
  std::pair<Collision, Collision> TestCollision(Collider *other);
  /// \brief Resolves a collision and passes the collision to the parent via OnCollision
  /// \param collision Collision to resolve
  void ResolveCollision(Collision collision);
  /// \brief Determines if a point is within the collider
  /// \param x World x position
  /// \param y World y position
  /// \return True if (x, y) is within the collider
  ///         False otherwise
  bool InCollider(int x, int y);

  /// \brief Gets the radius
  /// \return _radius
  double GetRadius();
  /// \brief Sets the radius
  /// \param radius New radius
  void SetRadius(double radius);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief Axis Aligned Bounding Box Collider class
class AABBCollider : public Collider
{
  /// \brief Width of the box
  double _width;
  /// \brief Height of the box
  double _height;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  AABBCollider(Object *parent = nullptr, std::string name = "AABBCollider");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param width Width of the collider
  /// \param height Height of the collider
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  AABBCollider(double width, double height, Object *parent = nullptr, std::string name = "AABBCollider");

  /// \brief Finds if there is a collision between this Collider and another Collider
  /// \param other Second collider
  /// \return Collisions found between the two objects
  ///         CANNOT_HANDLE is used to instead have other test against this Collider
  std::pair<Collision, Collision> TestCollision(Collider *other);
  /// \brief Resolves a collision and passes the collision to the parent via OnCollision
  /// \param collision Collision to resolve
  void ResolveCollision(Collision collision);
  /// \brief Determines if a point is within the collider
  /// \param x World x position
  /// \param y World y position
  /// \return True if (x, y) is within the collider
  ///         False otherwise
  bool InCollider(int x, int y);

  /// \brief Gets the width
  /// \return _width
  double GetWidth();
  /// \brief Sets the width
  /// \param width New width
  void SetWidth(double width);
  /// \brief Gets the height
  /// \return _height
  double GetHeight();
  /// \brief Sets the height
  /// \param height New height
  void SetHeight(double height);
  /// \brief Sets the box's size
  /// \param width New width
  /// \param height New height
  void SetSize(double width, double height);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};
/// \brief BoxCollider is a synonym to AABBCollider
typedef AABBCollider BoxCollider;
} // namespace Physics
} // namespace Aspen

#endif
