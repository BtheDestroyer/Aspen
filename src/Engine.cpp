#define __ENGINE_CPP

#include "Engine.hpp"
#include "Graphics.hpp"
#include "Event.hpp"
#include "Log.hpp"

#undef __ENGINE_CPP

namespace Aspen
{
namespace Engine
{
const Version::Version version(0, 1, 0, Version::TIER::PREALPHA);

Engine::Engine(Object *parent, std::string name)
  : Engine(START_FLAGS::NONE, parent, name)
{

}
Engine::Engine(int flags, Object *parent, std::string name)
    : Object(parent, name)
{
  if (flags & START_FLAGS::DEBUGGING_ON)
    _debugging = true;
  Log::Info("Creating Engine with the following flags:");
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
    if (flags & START_FLAGS::CREATE_TIME)
      Log::Info("  CREATE_TIME");
    if (flags & START_FLAGS::DEBUGGING_ON)
      Log::Info("  DEBUGGING_ON");

    if (flags & START_FLAGS::CREATE_GRAPHICS)
      CreateChild<Graphics::Graphics>();
    if (flags & START_FLAGS::CREATE_EVENTHANDLER)
    {
      Event::EventHandler *eh = CreateChild<Event::EventHandler>();
      if (flags & START_FLAGS::CREATE_EVENT_QUIT)
        eh->CreateChild<Event::QuitEventListener>();
      if (flags & START_FLAGS::CREATE_EVENT_KEYS_ALPHA)
        for (int i = static_cast<int>(SDLK_a); i <= static_cast<int>(SDLK_z); ++i)
          eh->AddChild(new Event::KeyEventListener(static_cast<SDL_Keycode>(i), eh, std::string("KeyEventListener-") + SDL_GetKeyName(i)));
      if (flags & START_FLAGS::CREATE_EVENT_KEYS_NUM)
        for (int i = static_cast<int>(SDLK_0); i <= static_cast<int>(SDLK_9); ++i)
          eh->AddChild(new Event::KeyEventListener(static_cast<SDL_Keycode>(i), eh, std::string("KeyEventListener-") + SDL_GetKeyName(i)));
      if (flags & START_FLAGS::CREATE_EVENT_KEYS_SPECIAL)
      {
        eh->AddChild(new Event::KeyEventListener(SDLK_SPACE, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_SPACE)));
        eh->AddChild(new Event::KeyEventListener(SDLK_ESCAPE, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_ESCAPE)));
        eh->AddChild(new Event::KeyEventListener(SDLK_BACKSPACE, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_BACKSPACE)));
        eh->AddChild(new Event::KeyEventListener(SDLK_DELETE, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_DELETE)));
        eh->AddChild(new Event::KeyEventListener(SDLK_SLASH, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_SLASH)));
        eh->AddChild(new Event::KeyEventListener(SDLK_BACKSLASH, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_BACKSLASH)));
        eh->AddChild(new Event::KeyEventListener(SDLK_PERIOD, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_PERIOD)));
        eh->AddChild(new Event::KeyEventListener(SDLK_COMMA, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_COMMA)));
        eh->AddChild(new Event::KeyEventListener(SDLK_CAPSLOCK, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_CAPSLOCK)));
        eh->AddChild(new Event::KeyEventListener(SDLK_SCROLLLOCK, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_SCROLLLOCK)));
        eh->AddChild(new Event::KeyEventListener(SDLK_NUMLOCKCLEAR, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_NUMLOCKCLEAR)));
        eh->AddChild(new Event::KeyEventListener(SDLK_RGUI, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_RGUI)));
        eh->AddChild(new Event::KeyEventListener(SDLK_LGUI, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_LGUI)));
        eh->AddChild(new Event::KeyEventListener(SDLK_LALT, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_LALT)));
        eh->AddChild(new Event::KeyEventListener(SDLK_RALT, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_RALT)));
        eh->AddChild(new Event::KeyEventListener(SDLK_LCTRL, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_LCTRL)));
        eh->AddChild(new Event::KeyEventListener(SDLK_RCTRL, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_RCTRL)));
        eh->AddChild(new Event::KeyEventListener(SDLK_MENU, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_MENU)));
        eh->AddChild(new Event::KeyEventListener(SDLK_SEMICOLON, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_SEMICOLON)));
        eh->AddChild(new Event::KeyEventListener(SDLK_QUOTE, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_QUOTE)));
        eh->AddChild(new Event::KeyEventListener(SDLK_MINUS, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_MINUS)));
        eh->AddChild(new Event::KeyEventListener(SDLK_EQUALS, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_EQUALS)));
        eh->AddChild(new Event::KeyEventListener(SDLK_LEFTBRACKET, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_LEFTBRACKET)));
        eh->AddChild(new Event::KeyEventListener(SDLK_RIGHTBRACKET, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_RIGHTBRACKET)));
        eh->AddChild(new Event::KeyEventListener(SDLK_LSHIFT, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_LSHIFT)));
        eh->AddChild(new Event::KeyEventListener(SDLK_RSHIFT, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_RSHIFT)));
        eh->AddChild(new Event::KeyEventListener(SDLK_TAB, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_TAB)));
        eh->AddChild(new Event::KeyEventListener(SDLK_RETURN, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_RETURN)));
        eh->AddChild(new Event::KeyEventListener(SDLK_LEFT, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_LEFT)));
        eh->AddChild(new Event::KeyEventListener(SDLK_RIGHT, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_RIGHT)));
        eh->AddChild(new Event::KeyEventListener(SDLK_UP, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_UP)));
        eh->AddChild(new Event::KeyEventListener(SDLK_DOWN, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_DOWN)));
        eh->AddChild(new Event::KeyEventListener(SDLK_HOME, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_HOME)));
        eh->AddChild(new Event::KeyEventListener(SDLK_END, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_END)));
        eh->AddChild(new Event::KeyEventListener(SDLK_PAGEDOWN, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_PAGEDOWN)));
        eh->AddChild(new Event::KeyEventListener(SDLK_PAGEUP, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_PAGEUP)));
        eh->AddChild(new Event::KeyEventListener(SDLK_INSERT, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_INSERT)));
        eh->AddChild(new Event::KeyEventListener(SDLK_PRINTSCREEN, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_PRINTSCREEN)));
      }
      if (flags & START_FLAGS::CREATE_EVENT_KEYS_FUNCTION)
      {
        eh->AddChild(new Event::KeyEventListener(SDLK_F1, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F1)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F2, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F2)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F3, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F3)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F4, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F4)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F5, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F5)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F6, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F6)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F7, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F7)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F8, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F8)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F9, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F9)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F10, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F10)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F11, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F11)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F12, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F12)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F13, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F13)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F14, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F14)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F15, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F15)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F16, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F16)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F17, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F17)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F18, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F18)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F19, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F19)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F20, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F20)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F21, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F21)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F22, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F22)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F23, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F23)));
        eh->AddChild(new Event::KeyEventListener(SDLK_F24, eh, std::string("KeyEventListener-") + SDL_GetKeyName(SDLK_F24)));
      }
    }
    if (flags & START_FLAGS::CREATE_TIME)
      CreateChild<Time::Time>();
  }
}
Engine::~Engine()
{
}

bool Engine::Debug()
{
  return _debugging;
}

void Engine::Debug(bool newval)
{
  _debugging = newval;
}
} // namespace Engine
} // namespace Aspen
