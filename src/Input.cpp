#define __INPUT_CPP

#include "Input.hpp"
#include <map>

#undef __INPUT_CPP

namespace Gladius
{
namespace Input
{
std::map<SDL_Keycode, Key> keys;

Key::Key() : held(false), pressed(false), released(false)
{
}

Key &GetKey(SDL_Keycode k)
{
  if (keys.find(k) == keys.end())
    keys[k] = Key();
  return keys[k];
}

bool KeyHeld(SDL_Keycode k)
{
  return GetKey(k).held;
}

bool KeyPressed(SDL_Keycode k)
{
  return GetKey(k).pressed;
}

bool KeyReleased(SDL_Keycode k)
{
  return GetKey(k).released;
}
} // namespace Input
} // namespace Gladius
