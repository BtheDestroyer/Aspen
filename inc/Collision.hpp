#ifndef __COLLISION_HPP
#define __COLLISION_HPP

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Physics namespace
namespace Physics
{
/// \brief Potential collision result values
enum COLLISION_RESULT
{
  /// \brief No collision occurred
  FAILURE,
  /// \brief A collision occurred
  SUCCESS,
  /// \brief The collider couldn't detect collision with the other Object
  CANNOT_HANDLE
};

/// \brief Forward declaration
class Collider;

/// \brief Collision class
class Collision
{
public:
  /// \brief Result of the tested collision
  ///        This class should be discarded unless this is SUCCESS
  COLLISION_RESULT result;
  /// \brief Second collider in a collision
  Collider *collider;
  /// \brief Relative x position of the collision
  double collisionX;
  /// \brief Relative y position of the collision
  double collisionY;
  /// \brief Angle the collision occurred at
  double collisionAngle;

  /// \brief Constructor
  /// \param other Second collider in the collision
  explicit Collision(Collider *other);
};
} // namespace Physics
} // namespace Aspen

#endif
