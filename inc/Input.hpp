#ifndef __INPUT_HPP
#define __INPUT_HPP
#include <SDL2/SDL.h>

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Stub namespace
///        To be replaced after running `make newfile NEWFILE=file`
namespace Input
{
class Key
{
public:
  bool held;
  bool pressed;
  bool released;
  Key();
};

Key &GetKey(SDL_Keycode k);

bool KeyHeld(SDL_Keycode k);
bool KeyPressed(SDL_Keycode k);
bool KeyReleased(SDL_Keycode k);
} // namespace Input
} // namespace Aspen

#endif