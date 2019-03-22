#ifndef __INPUT_HPP
#define __INPUT_HPP
#include <SDL2/SDL.h>
#include "Object.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Input namespace
///        Contains classes and functions for tracking and retrieving user input information
namespace Input
{
/// \brief Key status class
class Key
{
public:
  /// \brief True as long as the key is being pressed
  bool held;
  /// \brief True for the first frame key is being pressed
  bool pressed;
  /// \brief True for the frame key is released
  bool released;
  /// \brief Constructor
  Key();
};

/// \brief Mouse data
class Mouse
{
public:
  /// \brief World x position
  int x;
  /// \brief World y position
  int y;
  /// \brief Mouse wheel status
  int wheel;
  /// \brief Delta x since last frame
  int dx;
  /// \brief Delta y since last frame
  int dy;
  /// \brief Left mouse button status
  Key left;
  /// \brief Right mouse button status
  Key right;
  /// \brief Middle mouse button status
  Key middle;
};

/// \brief Input axis class
class Axis : public Object::Object
{
  /// \brief Positive key
  SDL_Keycode _pos;
  /// \brief Negative key
  SDL_Keycode _neg;
  /// \brief Lerp speed to 0
  float _gravity;
  /// \brief Lerp speed to 1/-1
  float _weight;
  /// \brief Current value of the axis
  float _value;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Axis(Object *parent = nullptr, std::string name = "Axis");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param positive Positive key
  /// \param negative Negative key
  /// \param gravity Lerp speed to 0
  /// \param weight Lerp speed to 1/-1
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Axis(SDL_Keycode positive = SDLK_UNKNOWN, SDL_Keycode negative = SDLK_UNKNOWN, float gravity = 0.5f, float weight = 0.5f, Object *parent = nullptr, std::string name = "Axis");

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Gets the current value of the axis
  /// \return _value
  float GetValue();

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief Gets a Key reference from a hidden map
///        Creates a new Key reference if one does not yet exist for the provided SDL_Keycode
Key &GetKey(SDL_Keycode k);

/// \brief Gets the held state of a Key
/// \param k Keycode to check
/// \return held state of the Key held at GetKey(k)
bool KeyHeld(SDL_Keycode k);
/// \brief Gets the pressed state of a Key
/// \param k Keycode to check
/// \return pressed state of the Key held at GetKey(k)
bool KeyPressed(SDL_Keycode k);
/// \brief Gets the released state of a Key
/// \param k Keycode to check
/// \return released state of the Key held at GetKey(k)
bool KeyReleased(SDL_Keycode k);

/// \brief Gets the mouse data
/// \return Mouse data
Mouse &GetMouse();
} // namespace Input
} // namespace Aspen

#endif
