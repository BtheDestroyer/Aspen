#define __AUDIO_CPP

#include <SDL2/SDL.h>
#include "Audio.hpp"
#include "Engine.hpp"

#undef __AUDIO_CPP

namespace Aspen
{
namespace Audio
{
unsigned Audio::_acount;

Audio::Audio(Object *parent, std::string name)
    : Object(parent, name)
{
  if (_acount == 0)
  {
    if (SDL_WasInit(Engine::SDL_INIT_FLAGS) != Engine::SDL_INIT_FLAGS)
    {
      Log::Error("SDL is not yet initialized! SDL_Error: %s", SDL_GetError());
      _valid = false;
      return;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
      Log::Error("Couldn't initialize SDL_mixer! Mix_Error: %s", Mix_GetError());
      _valid = false;
      return;
    }
  }

  ++_acount;
}

Audio::~Audio()
{
  for (Object *child : _children)
    delete child;
  _children.clear();
  End();
}

void Audio::End()
{
  if (!Valid())
    return;
  if (_acount-- == 1)
    Mix_Quit();
  Object::End();
}
} // namespace Audio
} // namespace Aspen
