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
/// \brief SoundEffect class
class SoundEffect : public Object::Object
{
  /// \brief Path to the soundeffect file
  std::string _path;
  /// \brief Loaded sound chunk
  Mix_Chunk *_sound;
  /// \brief Channels the sound has been played on
  std::vector<unsigned> _channels;

public:
  /// \brief Constructor
  /// \param parent Parent Object creating this Object
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  SoundEffect(Object *parent = nullptr, std::string name = "SoundEffect");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param path File path to load
  /// \param parent Parent Object creating this Object
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  SoundEffect(std::string path, Object *parent = nullptr, std::string name = "SoundEffect");

  /// \brief Shuts down and invalidates Object and all of its children
  ///        An invalid child Object will be deleted by their parent after they update
  void End();

  /// \brief Sets the file path and loads the new file
  /// \param path New path
  void SetPath(std::string path);
  /// \brief Gets the file path
  /// \return File path
  std::string GetPath();

  /// \brief Gets the unwrapped SDL_Mixer sound chunk
  /// \return _sound
  Mix_Chunk *GetSound();

  /// \brief Loads the current file path
  /// \return Success
  bool Load();

  /// \brief Plays the sound on a given channel
  /// \param channel Channel to play on
  ///                If this is -1, the first available channel will be selected
  void Play(int channel = -1);
  /// \brief Stops any instance of the sound currently playing
  void Stop();
  /// \brief Determines if the sound is playing on a given channel
  /// \param channel Channel to test
  /// \return True if the sound is currently playing on the channel
  ///         False otherwise
  bool IsPlayingOn(int channel);
  /// \brief Determines if the sound is playing on any channel
  /// \return True if the sound is currently playing on any channel
  ///         False otherwise
  bool IsPlaying();

  /// \brief Stops any instance of the sound on deactivation
  ///        This should be called or reimplemented by derived classes
  void OnDeactivate();
};

/// \brief Music class
class Music : public Object::Object
{
  /// \brief Tracks the last music file that was played
  static Music *_lastPlayed;
  /// \brief Path of this music file
  std::string _path;
  /// \brief loaded music file
  Mix_Music *_music;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Music(Object *parent = nullptr, std::string name = "Music");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param path File path to load
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Music(std::string path, Object *parent = nullptr, std::string name = "Music");

  /// \brief Shuts down and invalidates Object and all of its children
  ///        An invalid child Object will be deleted by their parent after they update
  void End();

  /// \brief Sets the file path and loads the new file
  /// \param path New path
  void SetPath(std::string path);
  /// \brief Gets the file path
  /// \return File path
  std::string GetPath();

  /// \brief Gets the unwrapped SDL_Mixer music
  /// \return _music
  Mix_Music *GetMusic();

  /// \brief Loads the music from the current path
  /// \return Success
  bool Load();
  /// \brief Plays this music
  /// \param loop If true, the music will loop forever
  ///             If false, the music will only play once
  /// \param fadeIn Time in seconds to fade in for
  void Play(bool loop = false, double fadeIn = 0.0);
  /// \brief Stops this music if is currently playing
  /// \param fadeOut Time in seconds to fade out for
  void Stop(double fadeOut = 0.0);
  /// \brief Determines if this music is currently playing
  /// \return True if this music is currently playing
  ///         False otherwise
  bool IsPlaying();

  /// \brief Stops the music from playing if it's currently playing
  void OnDeactivate();
};

/// \brief Audio class
class Audio : public Object::Object
{
  /// \brief Total number of Audio classes in existence
  static unsigned _acount;

public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Audio(Object *parent = nullptr, std::string name = "Audio");
  /// \brief Destructor
  ~Audio();

  /// \brief Shuts down and invalidates Object and all of its children
  ///        An invalid child Object will be deleted by their parent after they update
  void End();

  /// \brief Determines if any music is currently playing
  /// \return True if any music is currently playing
  ///         False otherwise
  bool IsPlayingMusic();
  /// \brief Stops any music currently playing
  /// \param fadeOut Time in seconds to fade out for
  void StopMusic(double fadeOut = 0.0);
};
} // namespace Audio
} // namespace Aspen

#endif
