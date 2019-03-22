#ifndef __COLLISION_HPP
#define __COLLISION_HPP

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Physics namespace
namespace Physics
{
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
  double collisionAngle;

  explicit Collision(Collider *other);
};
} // namespace Collision
} // namespace Aspen

#endif
