#define __AUDIO_CPP

#include <SDL2/SDL.h>
#include "Audio.hpp"
#include "Engine.hpp"

#undef __AUDIO_CPP

namespace Aspen
{
namespace Audio
{
SoundEffect::SoundEffect(Object *parent, std::string name)
    : SoundEffect("", parent, name)
{
}

SoundEffect::SoundEffect(std::string path, Object *parent, std::string name)
    : Object(parent, name), _path(path), _sound(nullptr), _playing(false)
{
  Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();
  if (!engine)
  {
    Log::Error("%s requires a root Engine with child Audio!", Name().c_str());
    _valid = false;
    return;
  }
  Audio *audio = engine->FindChildOfType<Audio>();
  if (!audio)
  {
    Log::Error("%s requires a root Engine with child Audio!", Name().c_str());
    _valid = false;
    return;
  }
  if (!audio->Valid())
  {
    Log::Error("%s found an invalid Audio! Audio was likely not initialized correctly.", Name().c_str());
    _valid = false;
    return;
  }
  Load();
}

void SoundEffect::End()
{
  if (_sound)
  {
    Mix_FreeChunk(_sound);
    _sound = nullptr;
  }
  Object::End();
}

void SoundEffect::SetPath(std::string path)
{
  _path = path;
  Load();
}

std::string SoundEffect::GetPath()
{
  return _path;
}

Mix_Chunk *SoundEffect::GetSound()
{
  return _sound;
}

bool SoundEffect::Load()
{
  if (!Valid())
    return false;
  if (_sound)
  {
    Mix_FreeChunk(_sound);
    _sound = nullptr;
  }

  _sound = Mix_LoadWAV(_path.c_str());
  if (!_sound)
  {
    Log::Error("%s faild to load sound from path: %s", Name().c_str(), _path.c_str());
    return false;
  }
  return true;
}

void SoundEffect::Play(int channel)
{
  if (!Valid())
    return;
  if (_sound)
    Mix_PlayChannel(channel, _sound, 0);
}

bool SoundEffect::IsPlaying()
{
  return _playing;
}

/////////////////////////////////////////////////////////

Music::Music(Object *parent, std::string name)
    : Music("", parent, name)
{
}

Music::Music(std::string path, Object *parent, std::string name)
    : Object(parent, name), _path(path), _music(nullptr), _playing(false)
{
  Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();
  if (!engine)
  {
    Log::Error("%s requires a root Engine with child Audio!", Name().c_str());
    _valid = false;
    return;
  }
  Audio *audio = engine->FindChildOfType<Audio>();
  if (!audio)
  {
    Log::Error("%s requires a root Engine with child Audio!", Name().c_str());
    _valid = false;
    return;
  }
  if (!audio->Valid())
  {
    Log::Error("%s found an invalid Audio! Audio was likely not initialized correctly.", Name().c_str());
    _valid = false;
    return;
  }
  Load();
}

void Music::End()
{
  if (_music)
  {
    Mix_FreeMusic(_music);
    _music = nullptr;
  }
  Object::End();
}

void Music::SetPath(std::string path)
{
  _path = path;
  Load();
}

std::string Music::GetPath()
{
  return _path;
}

Mix_Music *Music::GetMusic()
{
  return _music;
}

bool Music::Load()
{
  if (!Valid())
    return false;
  if (_music)
  {
    Mix_FreeMusic(_music);
    _music = nullptr;
  }

  _music = Mix_LoadMUS(_path.c_str());
  if (!_music)
  {
    Log::Error("%s faild to load music from path: %s", Name().c_str(), _path.c_str());
    return false;
  }
  return true;
}

void Music::Play(bool loop, double fadeIn)
{
  if (!Valid())
    return;
  if (_music)
  {
    if (fadeIn <= 0.0)
      Mix_PlayMusic(_music, loop ? 0 : -1);
    else
      Mix_FadeInMusic(_music, loop ? 0 : -1, int(fadeIn * 1000));
  }
}

void Music::Stop(double fadeOut)
{
  if (!Valid())
    return;
  Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();
  if (!engine)
  {
    Log::Error("%s requires a root Engine with child Audio!", Name().c_str());
    return;
  }
  Audio *audio = engine->FindChildOfType<Audio>();
  if (!audio)
  {
    Log::Error("%s requires a root Engine with child Audio!", Name().c_str());
    return;
  }
  audio->StopMusic(fadeOut);
}

bool Music::IsPlaying()
{
  return Mix_PlayingMusic();
}

/////////////////////////////////////////////////////////

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

bool Audio::IsPlayingMusic()
{
  return Mix_PlayingMusic();
}

void Audio::StopMusic(double fadeOut)
{
  if (IsPlayingMusic())
  {
    if (fadeOut <= 0.0)
      Mix_HaltMusic();
    else
      Mix_FadeOutMusic(int(fadeOut * 1000));
  }
}
} // namespace Audio
} // namespace Aspen
