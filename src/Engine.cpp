#define __ENGINE_CPP

#include "Engine.hpp"
#include "Log.hpp"

#undef __ENGINE_CPP

namespace Aspen
{
namespace Engine
{
const Version::Version version(0, 1, 0, Version::TIER::PREALPHA);

Engine::Engine(int flags)
    : Object(nullptr, "Engine")
{
  std::stringstream str;
  str << "Creating Engine with the following flags:";
  Log::Info(str);
  if (flags == START_FLAGS::NONE)
    Log::Info("  NONE");
  else
  {
    if (flags & START_FLAGS::CREATE_GRAPHICS)
      Log::Info("  CREATE_GRAPHICS");
    if (flags & START_FLAGS::CREATE_EVENTHANDLER)
      Log::Info("  CREATE_EVENTHANDLER");
    if (flags & START_FLAGS::CREATE_EVENT_QUIT)
      Log::Info("  CREATE_EVENT_QUIT");
    if (flags & START_FLAGS::CREATE_EVENT_KEYS_ALPHA)
    {
      Log::Info("  CREATE_EVENT_KEYS_ALPHA");
      if (!(flags & START_FLAGS::CREATE_EVENTHANDLER))
        Log::Warning("    This only works if you are using CREATE_EVENTHANDLER");
    }
    if (flags & START_FLAGS::CREATE_EVENT_KEYS_NUM)
    {
      Log::Info("  CREATE_EVENT_KEYS_NUM");
      if (!(flags & START_FLAGS::CREATE_EVENTHANDLER))
        Log::Warning("    This only works if you are using CREATE_EVENTHANDLER");
    }
    if (flags & START_FLAGS::CREATE_EVENT_KEYS_SPECIAL)
    {
      Log::Info("  CREATE_EVENT_KEYS_SPECIAL");
      if (!(flags & START_FLAGS::CREATE_EVENTHANDLER))
        Log::Warning("    This only works if you are using CREATE_EVENTHANDLER");
    }
    if (flags & START_FLAGS::CREATE_EVENT_KEYS_FUNCTION)
    {
      Log::Info("  CREATE_EVENT_KEYS_SPECIAL");
      if (!(flags & START_FLAGS::CREATE_EVENTHANDLER))
        Log::Warning("    This only works if you are using CREATE_EVENTHANDLER");
    }

    if (flags & START_FLAGS::CREATE_GRAPHICS)
      _graphics = CreateChild<Graphics::Graphics>();
    if (flags & START_FLAGS::CREATE_EVENTHANDLER)
    {
      _eventhandler = CreateChild<Event::EventHandler>();
      if (flags & START_FLAGS::CREATE_EVENT_QUIT)
        _eventhandler->CreateChild<Event::QuitEventListener>();
      if (flags & START_FLAGS::CREATE_EVENT_KEYS_ALPHA)
        for (int i = static_cast<int>(SDLK_a); i <= static_cast<int>(SDLK_z); ++i)
          _eventhandler->AddChild(new Event::KeyEventListener(static_cast<SDL_Keycode>(i)));
      if (flags & START_FLAGS::CREATE_EVENT_KEYS_NUM)
        for (int i = static_cast<int>(SDLK_0); i <= static_cast<int>(SDLK_9); ++i)
          _eventhandler->AddChild(new Event::KeyEventListener(static_cast<SDL_Keycode>(i)));
      if (flags & START_FLAGS::CREATE_EVENT_KEYS_SPECIAL)
      {
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_SPACE));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_ESCAPE));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_BACKSPACE));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_DELETE));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_SLASH));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_BACKSLASH));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_PERIOD));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_COMMA));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_CAPSLOCK));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_SCROLLLOCK));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_NUMLOCKCLEAR));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_RGUI));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_LGUI));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_LALT));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_RALT));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_LCTRL));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_RCTRL));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_MENU));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_SEMICOLON));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_QUOTE));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_MINUS));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_EQUALS));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_LEFTBRACKET));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_RIGHTBRACKET));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_LSHIFT));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_RSHIFT));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_TAB));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_RETURN));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_LEFT));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_RIGHT));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_UP));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_DOWN));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_HOME));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_END));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_PAGEDOWN));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_PAGEUP));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_INSERT));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_PRINTSCREEN));
      }
      if (flags & START_FLAGS::CREATE_EVENT_KEYS_FUNCTION)
      {
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F1));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F2));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F3));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F4));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F5));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F6));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F7));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F8));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F9));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F10));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F11));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F12));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F13));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F14));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F15));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F16));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F17));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F18));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F19));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F20));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F21));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F22));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F23));
        _eventhandler->AddChild(new Event::KeyEventListener(SDLK_F24));
      }
    }
  }
}
Engine::~Engine()
{
}

void Engine::RefreshGraphics()
{
  _graphics = FindChildOfType<Graphics::Graphics>();
}

Graphics::Graphics *Engine::Graphics()
{
  return _graphics;
}

void Engine::RefreshEventHandler()
{
  _eventhandler = FindChildOfType<Event::EventHandler>();
}

Event::EventHandler *Engine::EventHandler()
{
  return _eventhandler;
}

void Engine::RemoveChild(Object *child)
{
  if (child == _graphics)
    _graphics = nullptr;
  if (child == _eventhandler)
    _eventhandler = nullptr;
  Object::RemoveChild(child);
}

void Engine::RemoveChild(unsigned index)
{
  if (_children.size() < index)
  {
    if (_children[index] == _graphics)
      _graphics = nullptr;
    if (_children[index] == _eventhandler)
      _eventhandler = nullptr;
    Object::RemoveChild(index);
  }
}
} // namespace Engine
} // namespace Aspen
