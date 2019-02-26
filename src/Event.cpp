#define __EVENT_CPP

#include "Event.hpp"
#include "Log.hpp"
#include "Input.hpp"
#include "Engine.hpp"
#include "imgui.h"

#undef __EVENT_CPP

namespace Aspen
{
namespace Event
{
EventListener::EventListener(Object *parent, std::string name)
    : Object(parent, name)
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

void EventListener::PopulateDebugger()
{
  Object::PopulateDebugger();
}

QuitEventListener::QuitEventListener(Object *parent, std::string name)
    : EventListener(parent, name)
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

void QuitEventListener::PopulateDebugger()
{
  EventListener::PopulateDebugger();
}

KeyEventListener::KeyEventListener(Object *parent, std::string name)
    : EventListener(parent, name), _key(SDLK_UNKNOWN)
{
}

KeyEventListener::KeyEventListener(SDL_Keycode k, Object *parent, std::string name)
    : EventListener(parent, name), _key(k)
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
        if (FindAncestorOfType<Engine::Engine>() && FindAncestorOfType<Engine::Engine>()->Debug())
          Log::Debug("Key down: %s", SDL_GetKeyName(_key));
      }
      else if (event->type == SDL_KEYUP && k.held)
      {
        k.held = false;
        k.released = true;
        if (FindAncestorOfType<Engine::Engine>() && FindAncestorOfType<Engine::Engine>()->Debug())
          Log::Debug("Key up: %s", SDL_GetKeyName(_key));
      }
    }
  }
}

void KeyEventListener::PopulateDebugger()
{
  ImGui::Text("Key: %s", SDL_GetKeyName(_key));
  EventListener::PopulateDebugger();
}

EventHandler::EventHandler(Object *parent, std::string name)
    : Object(parent, name)
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

void EventHandler::PopulateDebugger()
{
  Object::PopulateDebugger();
}
} // namespace Event
} // namespace Aspen
