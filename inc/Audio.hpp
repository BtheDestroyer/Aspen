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
  Mix_Chunk *_sound;
};

class Music : public Object::Object
{
  Mix_Music *_music;
};

class Audio : public Object::Object
{
  static unsigned _acount;
public:
  Audio(Object *parent = nullptr, std::string name = "Audio");
  ~Audio();

  void End();
};
} // namespace Audio
} // namespace Aspen

#endif
