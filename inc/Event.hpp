#ifndef __EVENT_HPP
#define __EVENT_HPP
#include "Object.hpp"
#include <SDL2/SDL.h>

namespace Gladius
{
namespace Event
{
class EventListener : public Object::Object
{
public:
  EventListener();
  ~EventListener();

  void operator()(SDL_Event *event = nullptr);

  virtual void Handle(SDL_Event *event);
};

class QuitEventListener : public EventListener
{
public:
  QuitEventListener();
  ~QuitEventListener();

  void Handle(SDL_Event *event);
};

class KeyEventListener : public EventListener
{
  SDL_Keycode _key;

public:
  KeyEventListener(SDL_Keycode k = SDLK_UNKNOWN);
  ~KeyEventListener();

  void SetKey(SDL_Keycode k);

  void Handle(SDL_Event *event);
};

class EventHandler : public Object::Object
{
public:
  EventHandler();
  ~EventHandler();
  void operator()();
};
} // namespace Event
} // namespace Gladius

#endif
