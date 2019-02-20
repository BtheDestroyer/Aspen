#define __EVENT_CPP

#include "Event.hpp"
#include "Log.hpp"
#include "Input.hpp"

#undef __EVENT_CPP

namespace Aspen
{
namespace Event
{
EventListener::EventListener()
    : Object()
{
}

EventListener::~EventListener()
{
}

void EventListener::operator()(SDL_Event *event)
{
  if (!event)
    Object::operator()();
  else
    Handle(event);
}

void EventListener::Handle(SDL_Event *event)
{
}

QuitEventListener::QuitEventListener()
    : EventListener()
{
}

QuitEventListener::~QuitEventListener()
{
}

void QuitEventListener::Handle(SDL_Event *event)
{
  if (event && event->type == SDL_QUIT)
    Root()->End();
}

KeyEventListener::KeyEventListener(SDL_Keycode k)
    : EventListener(), _key(k)
{
}

KeyEventListener::~KeyEventListener()
{
}

void KeyEventListener::SetKey(SDL_Keycode k)
{
  _key = k;
}

void KeyEventListener::Handle(SDL_Event *event)
{
  if (event && _key != SDLK_UNKNOWN)
  {
    Input::Key &k = Input::GetKey(_key);
    if (k.pressed)
      k.pressed = false;
    if (k.released)
      k.released = false;

    if (event->key.keysym.sym == _key)
    {
      if (event->type == SDL_KEYDOWN && !k.held)
      {
        k.pressed = true;
        k.held = true;
        std::stringstream str;
        str << "Key down: " << SDL_GetKeyName(_key);
        Log::Debug(str);
      }
      else if (event->type == SDL_KEYUP && k.held)
      {
        k.held = false;
        k.released = true;
        std::stringstream str;
        str << "Key up: " << SDL_GetKeyName(_key);
        Log::Debug(str);
      }
    }
  }
}

EventHandler::EventHandler()
    : Object()
{
}

EventHandler::~EventHandler()
{
}

void EventHandler::operator()()
{
  if (!Valid())
    return;
  Object::operator()();
  SDL_Event event;
  std::vector<EventListener *> listeners = FindChildrenOfType<EventListener>();
  while (SDL_PollEvent(&event))
    for (EventListener *el : listeners)
      (*el)(&event);
}

} // namespace Event
} // namespace Aspen
