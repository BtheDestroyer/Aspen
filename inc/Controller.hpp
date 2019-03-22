#ifndef __CONTROLLER_HPP
#define __CONTROLLER_HPP

#include <string>
#include <SDL2/SDL_Keycode.h>
#include "Object.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Controller namespace
namespace Controller
{
/// \brief 8 directional player controller class
class PlayerController_8Way : public Object::Object
{
  /// \brief Acceleration strength
  double _acceleration;
  /// \brief Max speed
  double _speed;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  PlayerController_8Way(Object *parent = nullptr, std::string name = "PlayerController_8Way");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param up Up key
  /// \param down Down key
  /// \param left Left key
  /// \param right Right key
  /// \param speed Max speed
  /// \param acceleration Acceleration
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  PlayerController_8Way(SDL_Keycode up, SDL_Keycode down, SDL_Keycode left, SDL_Keycode right, double speed, double acceleration, Object *parent = nullptr, std::string name = "PlayerController_8Way");

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Gets the controller's acceleration
  /// \return _acceleration
  double GetAcceleration();
  /// \brief Sets the controller's acceleration
  /// \param acc New acceleration
  void SetAcceleration(double acc);

  /// \brief Gets the controller's max speed
  /// \return _speed
  double GetSpeed();
  /// \brief Sets the controller's max speed
  /// \param speed New max speed
  void SetSpeed(double speed);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief Sidescroller player controller class
class PlayerController_Sidescroller : public Object::Object
{
  /// \brief Acceleration strength
  double _acceleration;
  /// \brief Max speed
  double _speed;
  /// \brief Jump strength
  double _jumpStrength;
  /// \brief Jump height
  double _jumpHeight;
  /// \brief Remaining jump height
  double _jumpRemaining;
  /// \brief Jump key
  SDL_Keycode _jumpKey;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  PlayerController_Sidescroller(Object *parent = nullptr, std::string name = "PlayerController_Sidescroller");
  /// \brief Constructor
  /// \param left Left key
  /// \param right Right key
  /// \param jump Jump key
  /// \param speed Max speed
  /// \param acceleration Acceleration
  /// \param jumpStrength Jump strength
  /// \param jumpHeight Jump height
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  PlayerController_Sidescroller(SDL_Keycode left, SDL_Keycode right, SDL_Keycode jump, double speed, double acceleration, double jumpStrength, double jumpHeight, Object *parent = nullptr, std::string name = "PlayerController_Sidescroller");

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Gets the controller's acceleration
  /// \return _acceleration
  double GetAcceleration();
  /// \brief Sets the controller's acceleration
  /// \param acc New acceleration
  void SetAcceleration(double acc);

  /// \brief Gets the controller's max speed
  /// \return _speed
  double GetSpeed();
  /// \brief Sets the controller's max speed
  /// \param speed New max speed
  void SetSpeed(double speed);

  /// \brief Gets the controller's jump strength
  /// \return _jumpStrength
  double GetJumpStrength();
  /// \brief Sets the controller's jump strength
  /// \param strength New jump strength
  void SetJumpStrength(double strength);

  /// \brief Gets the controller's jump height
  /// \return _jumpHeight
  double GetJumpHeight();
  /// \brief Sets the controller's jump height
  /// \param height New jump height
  void SetJumpHeight(double height);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};
} // namespace Controller
} // namespace Aspen

#endif
