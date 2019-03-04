#ifndef __AUDIO_HPP
#define __AUDIO_HPP

#include <SDL2/SDL_mixer.h>
#include "Object.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Audio namespace
namespace Audio
{
class SoundEffect : public Object::Object
{
  std::string _path;
  Mix_Chunk *_sound;
  bool _playing;

public:
  SoundEffect(Object *parent = nullptr, std::string name = "SoundEffect");
  SoundEffect(std::string path, Object *parent = nullptr, std::string name = "SoundEffect");

  void End();

  void SetPath(std::string path);
  std::string GetPath();

  Mix_Chunk *GetSound();

  bool Load();
  void Play(int channel = -1);
  bool IsPlaying();
};

class Music : public Object::Object
{
  std::string _path;
  Mix_Music *_music;
  bool _playing;

public:
  Music(Object *parent = nullptr, std::string name = "Music");
  Music(std::string path, Object *parent = nullptr, std::string name = "Music");

  void End();

  void SetPath(std::string path);
  std::string GetPath();

  Mix_Music *GetMusic();

  bool Load();
  void Play(bool loop = false, double fadeIn = 0.0);
  void Stop(double fadeOut = 0.0);
  bool IsPlaying();
};

class Audio : public Object::Object
{
  static unsigned _acount;

public:
  Audio(Object *parent = nullptr, std::string name = "Audio");
  ~Audio();

  void End();

  bool IsPlayingMusic();
  void StopMusic(double fadeOut = 0.0);
};
} // namespace Audio
} // namespace Aspen

#endif
