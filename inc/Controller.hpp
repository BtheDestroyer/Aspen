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
public:
  PlayerController_8Way(Object *parent = nullptr, std::string name = "PlayerController_8Way");
  PlayerController_8Way(SDL_Keycode up, SDL_Keycode down, SDL_Keycode left, SDL_Keycode right, Object *parent = nullptr, std::string name = "PlayerController_8Way");

  void operator()();
};
} // namespace Controller
} // namespace Aspen

#endif
