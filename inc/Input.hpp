#ifndef __INPUT_HPP
#define __INPUT_HPP
#include <SDL2/SDL.h>

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
} // namespace Input
} // namespace Aspen

#endif