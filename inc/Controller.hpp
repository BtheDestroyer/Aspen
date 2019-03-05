#ifndef __CONTROLLER_HPP
#define __CONTROLLER_HPP

#include <string>
#include <SDL2/SDL_Keycode.h>
#include "Object.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Controller namespace
///        To be replaced after running `make newfile NEWFILE=file`
namespace Controller
{
class PlayerController_8Way : public Object::Object
{
  double _acceleration;
  double _speed;

public:
  PlayerController_8Way(Object *parent = nullptr, std::string name = "PlayerController_8Way");
  PlayerController_8Way(SDL_Keycode up, SDL_Keycode down, SDL_Keycode left, SDL_Keycode right, double speed, double acceleration, Object *parent = nullptr, std::string name = "PlayerController_8Way");

  void operator()();

  void SetAcceleration(double acc);
  double GetAcceleration();

  void SetSpeed(double speed);
  double GetSpeed();

  void PopulateDebugger();
};

class PlayerController_Sidescroller : public Object::Object
{
  double _acceleration;
  double _speed;
  double _jumpStrength;
  double _jumpHeight;
  double _jumpRemaining;
  SDL_Keycode _jumpKey;

public:
  PlayerController_Sidescroller(Object *parent = nullptr, std::string name = "PlayerController_Sidescroller");
  PlayerController_Sidescroller(SDL_Keycode left, SDL_Keycode right, SDL_Keycode jump, double speed, double acceleration, double jumpStrength, double jumpHeight, Object *parent = nullptr, std::string name = "PlayerController_Sidescroller");

  void operator()();

  void SetAcceleration(double acc);
  double GetAcceleration();

  void SetSpeed(double speed);
  double GetSpeed();

  void SetJumpStrength(double strength);
  double GetJumpStrength();

  void SetJumpHeight(double height);
  double GetJumpHeight();

  void PopulateDebugger();
};
} // namespace Controller
} // namespace Aspen

#endif
