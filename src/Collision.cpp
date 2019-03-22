#define __COLLISION_CPP

#include "Collision.hpp"

#undef __COLLISION_CPP

namespace Aspen
{
namespace Physics
{
Collision::Collision(Collider *other)
    : collider(other), collisionX(0), collisionY(0), collisionAngle(0)
{
}
} // namespace Physics
} // namespace Aspen
