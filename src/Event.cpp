#define __EVENT_CPP

#include "Event.hpp"
#include "Log.hpp"
#include "Input.hpp"
#include "Graphics.hpp"
#include "Engine.hpp"
#include "Transform.hpp"
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

void KeyEventListener::operator()()
{
  Input::Key &k = Input::GetKey(_key);
  if (k.pressed)
    k.pressed = false;
  if (k.released)
    k.released = false;
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
    if (event->key.keysym.sym == _key)
    {
      if (event->type == SDL_KEYDOWN && !k.held)
      {
        k.pressed = true;
        k.held = true;
        if (Engine::Engine::Get() && Engine::Engine::Get()->Debug())
          Log::Debug("Key down: %s", SDL_GetKeyName(_key));
      }
      else if (event->type == SDL_KEYUP && k.held)
      {
        k.held = false;
        k.released = true;
        if (Engine::Engine::Get() && Engine::Engine::Get()->Debug())
          Log::Debug("Key up: %s", SDL_GetKeyName(_key));
      }
    }
  }
}

void KeyEventListener::PopulateDebugger()
{
  ImGui::Text("Key: %s", SDL_GetKeyName(_key));
  Input::Key &k = Input::GetKey(_key);
  ImGui::Text("Status: [%s] [%s] [%s]", k.pressed ? "Pressed" : "", k.held ? "Held" : "", k.released ? "Released" : "");
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
  if (!Active())
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

MouseEventListener::MouseEventListener(Object *parent, std::string name)
    : EventListener(parent, name), _m(Input::GetMouse())
{
}

MouseEventListener::~MouseEventListener()
{
  _m.x = 0;
  _m.y = 0;
  _m.wheel = 0;
  _m.dx = 0;
  _m.dy = 0;
  _m.left.held = false;
  _m.left.pressed = false;
  _m.left.released = false;
  _m.right.held = false;
  _m.right.pressed = false;
  _m.right.released = false;
  _m.middle.held = false;
  _m.middle.pressed = false;
  _m.middle.released = false;
}

void MouseEventListener::operator()()
{
  _m.dx = 0;
  _m.dy = 0;
  _m.wheel = 0;
  if (_m.left.pressed)
    _m.left.pressed = false;
  if (_m.right.pressed)
    _m.right.pressed = false;
  if (_m.middle.pressed)
    _m.middle.pressed = false;
  if (_m.left.released)
    _m.left.released = false;
  if (_m.right.released)
    _m.right.released = false;
  if (_m.middle.released)
    _m.middle.released = false;
  int oldx = _m.x;
  int oldy = _m.y;
  SDL_GetMouseState(&_m.x, &_m.y);
  _m.dx = _m.x - oldx;
  _m.dy = _m.y - oldy;
  Object::operator()();
}

void MouseEventListener::Handle(SDL_Event *event)
{
  if (event->type == SDL_MOUSEBUTTONDOWN)
  {
    if (event->button.button == SDL_BUTTON_LEFT && !_m.left.held)
    {
      _m.left.pressed = true;
      _m.left.held = true;
      Log::Debug("LMB down");
    }
    else if (event->button.button == SDL_BUTTON_RIGHT && !_m.right.held)
    {
      _m.right.pressed = true;
      _m.right.held = true;
      Log::Debug("RMB down");
    }
    else if (event->button.button == SDL_BUTTON_MIDDLE && !_m.middle.held)
    {
      _m.middle.pressed = true;
      _m.middle.held = true;
      Log::Debug("MMB down");
    }
  }
  else if (event->type == SDL_MOUSEBUTTONUP)
  {
    if (event->button.button == SDL_BUTTON_LEFT && _m.left.held)
    {
      _m.left.released = true;
      _m.left.held = false;
      Log::Debug("LMB up");
    }
    else if (event->button.button == SDL_BUTTON_RIGHT && _m.right.held)
    {
      _m.right.released = true;
      _m.right.held = false;
      Log::Debug("RMB up");
    }
    else if (event->button.button == SDL_BUTTON_MIDDLE && _m.middle.held)
    {
      _m.middle.released = true;
      _m.middle.held = false;
      Log::Debug("MMB up");
    }
  }
  else if (event->type == SDL_MOUSEWHEEL)
  {
    _m.wheel = event->wheel.y;
  }
}

void MouseEventListener::PopulateDebugger()
{
  ImGui::Text("World Position: (%d, %d)", _m.x, _m.y);
  ImGui::Text("Delta: (%d, %d)", _m.dx, _m.dy);
  ImGui::Text("Wheel: %d", _m.wheel);
  ImGui::Text("LMB: [%s] [%s] [%s]", _m.left.pressed ? "Pressed" : "", _m.left.held ? "Held" : "", _m.left.released ? "Released" : "");
  ImGui::Text("RMB: [%s] [%s] [%s]", _m.right.pressed ? "Pressed" : "", _m.right.held ? "Held" : "", _m.right.released ? "Released" : "");
  ImGui::Text("MMB: [%s] [%s] [%s]", _m.middle.pressed ? "Pressed" : "", _m.middle.held ? "Held" : "", _m.middle.released ? "Released" : "");
  EventListener::PopulateDebugger();
}
} // namespace Event
} // namespace Aspen
