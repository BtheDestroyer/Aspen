#define __ENGINE_CPP

#include "Engine.hpp"
#include "Time.hpp"
#include "Graphics.hpp"
#include "Event.hpp"
#include "Physics.hpp"
#include "Debug.hpp"
#include "Log.hpp"
#include "GameState.hpp"
#include "Audio.hpp"
#include "imgui.h"
#include <SDL2/SDL.h>

#undef __ENGINE_CPP

namespace Aspen
{
namespace Engine
{
const Version::Version VERSION(0, 2, 0, Version::TIER::PREALPHA);
const unsigned SDL_INIT_FLAGS = SDL_INIT_VIDEO | SDL_INIT_AUDIO;

unsigned Engine::_ecount = 0;
Engine *Engine::_main = nullptr;

Engine::Engine(Object *parent, std::string name)
    : Engine(START_FLAGS::NONE, parent, name)
{
}
Engine::Engine(int flags, Object *parent, std::string name)
    : Object(parent, name), _debugging(flags & START_FLAGS::DEBUGGING_ON)
{
  if (_ecount == 0)
  {
    if (SDL_Init(SDL_INIT_FLAGS) < 0)
    {
      Log::Error("Could not initialize SDL. SDL_Error: %s", SDL_GetError());
      _valid = false;
      return;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
  }

  Log::Info("Creating Engine with the following flags:");
  if (flags == START_FLAGS::NONE)
    Log::Info("  NONE");
  else
  {
    if (flags & START_FLAGS::CREATE_GRAPHICS)
      Log::Info("  CREATE_GRAPHICS");
    if (flags & START_FLAGS::CREATE_GRAPHICS_DEBUGGER)
    {
      Log::Info("  CREATE_GRAPHICS_DEBUGGER");
      if (!(flags & START_FLAGS::CREATE_GRAPHICS))
        Log::Warning("    This only works if you are using CREATE_GRAPHICS");
    }
    if (flags & START_FLAGS::CREATE_GRAPHICS_FONTCACHE)
    {
      Log::Info("  CREATE_GRAPHICS_FONTCACHE");
      if (!(flags & START_FLAGS::CREATE_GRAPHICS))
        Log::Warning("    This only works if you are using CREATE_GRAPHICS");
    }
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
    if (flags & START_FLAGS::CREATE_EVENT_MOUSE)
    {
      Log::Info("  CREATE_EVENT_MOUSE");
      if (!(flags & START_FLAGS::CREATE_EVENTHANDLER))
        Log::Warning("    This only works if you are using CREATE_EVENTHANDLER");
    }
    if (flags & START_FLAGS::CREATE_TIME)
      Log::Info("  CREATE_TIME");
    if (flags & START_FLAGS::CREATE_PHYSICS)
      Log::Info("  CREATE_PHYSICS");
    if (flags & START_FLAGS::CREATE_GAMESTATEMANAGER)
      Log::Info("  CREATE_GAMESTATEMANAGER");
    if (flags & START_FLAGS::CREATE_AUDIO)
      Log::Info("  CREATE_AUDIO");
    if (flags & START_FLAGS::DEBUGGING_ON)
      Log::Info("  DEBUGGING_ON");

    if (flags & START_FLAGS::CREATE_GRAPHICS)
    {
      Graphics::Graphics *gfx = CreateChild<Graphics::Graphics>();
      if (flags & START_FLAGS::CREATE_GRAPHICS_DEBUGGER)
        gfx->CreateChild<Debug::Debug>();
      if (flags & START_FLAGS::CREATE_GRAPHICS_FONTCACHE)
        gfx->CreateChild<Graphics::FontCache>();
    }
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
      if (flags & START_FLAGS::CREATE_EVENT_MOUSE)
        eh->CreateChild<Event::MouseEventListener>();
    }
    if (flags & START_FLAGS::CREATE_TIME)
      CreateChild<Time::Time>();
    if (flags & START_FLAGS::CREATE_PHYSICS)
      CreateChild<Physics::Physics>();
    if (flags & START_FLAGS::CREATE_GAMESTATEMANAGER)
      CreateChild<GameState::GameStateManager>();
    if (flags & START_FLAGS::CREATE_AUDIO)
      CreateChild<Audio::Audio>();
  }

  ++_ecount;
  if (!_main)
    _main = this;
}

Engine::~Engine()
{
  End();
  for (Object *child : _children)
    delete child;
  _children.clear();
  if (_main == this)
    _main = nullptr;
  if (_ecount-- == 1 && SDL_WasInit(SDL_INIT_FLAGS) == SDL_INIT_FLAGS)
    SDL_Quit();
}

Engine *Engine::Get()
{
  return _main;
}

void Engine::operator()()
{
  if (!Active())
    return;
  OnEarlyUpdate();
  Object::operator()();
  OnLateUpdate();
}

bool Engine::Debug()
{
  return _debugging;
}

void Engine::Debug(bool newval)
{
  _debugging = newval;
}

void Engine::PopulateDebugger()
{
  ImGui::Text("Debugging: %s", Debug() ? "True" : "False");
  Object::PopulateDebugger();
}
} // namespace Engine
} // namespace Aspen
