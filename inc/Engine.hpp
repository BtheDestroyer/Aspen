#ifndef __ENGINE_HPP
#define __ENGINE_HPP
#include "Graphics.hpp"
#include "Event.hpp"
#include "Version.hpp"
#include "Time.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Engine namespace
///        Holds the core Engine class
namespace Engine
{
/// \brief Current engine version
extern const Version::Version version;

/// \brief START_FLAGS namespace
///        Contains const ints to be passed to Engine's constructor
///        Combine START_FLAGS parameters by using logical or (param1 | param2)
namespace START_FLAGS
{
/// \brief Does nothing
const int NONE                       = 0b0000000000000000;
/// \brief Creates a Graphics::Graphics Object with default window size as a child
const int CREATE_GRAPHICS            = 0b0000000000000001;
/// \brief Creates an Event::EventHandler Object as a child
const int CREATE_EVENTHANDLER        = 0b0000000000000010;
/// \brief Gives the child EventHandler a QuitEventListener
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_QUIT          = 0b0000000000000100;
/// \brief Gives the child EventHandler a KeyEventListener for each alphabet key
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_KEYS_ALPHA    = 0b0000000000001000;
/// \brief Gives the child EventHandler a KeyEventListener for each numeric key
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_KEYS_NUM      = 0b0000000000010000;
/// \brief Gives the child EventHandler a KeyEventListener for each special key
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_KEYS_SPECIAL  = 0b0000000000100000;
/// \brief Gives the child EventHandler a KeyEventListener for each function key
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_KEYS_FUNCTION = 0b0000000001000000;
/// \brief Synonym for (CREATE_EVENT_KEYS_ALPHA | CREATE_EVENT_KEYS_NUM | CREATE_EVENT_KEYS_SPECIAL | CREATE_EVENT_KEYS_FUNCTION | CREATE_EVENT_KEYS_ALL)
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_KEYS_ALL      = 0b0000000001111000;
/// \brief Synonym for (CREATE_EVENT_QUIT | CREATE_EVENT_KEYS_ALL)
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_ALL           = 0b0000000001111100;
/// \brief Creates an Time::Time Object as a child
const int CREATE_TIME                = 0b0000000010000000;
/// \brief Determines if children of the engine should debug
const int DEBUGGING_ON               = 0b1000000000000000;
/// \brief Synonym for all START_FLAGS
const int ALL                        = 0b1111111111111111;
} // namespace START_FLAGS

/// \brief Engine class
class Engine : public Object::Object
{
  /// \brief Primary Graphics child
  Graphics::Graphics *_graphics;
  /// \brief Primary EventHandler child
  Event::EventHandler *_eventhandler;
  Time::Time *_time;
  bool _debugging = false;

public:
  /// \brief Constructor
  /// \param flags START_FLAGS to instantiate the Engine with
  Engine(int flags = START_FLAGS::NONE);
  /// \brief Destructor
  ~Engine();

  /// \brief Refreshes _graphics if necessary
  void RefreshGraphics();
  /// \brief Gets the primary Graphics child
  /// \return _graphics
  Graphics::Graphics *Graphics();
  /// \brief Refreshes _eventhandler if necessary
  void RefreshEventHandler();
  /// \brief Gets the primary EventHandler child
  /// \return _eventhandler
  Event::EventHandler *EventHandler();
  void RefreshTime();
  Time::Time *Time();

  /// \brief Wrapper for Object::RemoveChild(Object *child)
  ///        Sets _graphics to nullptr if it's removed
  ///        Sets _eventhandler to nullptr if it's removed
  /// \param child Child to remove
  void RemoveChild(Object *child);
  /// \brief Wrapper for Object::RemoveChild(int index)
  ///        Sets _graphics to nullptr if it's removed
  ///        Sets _eventhandler to nullptr if it's removed
  /// \param index Index of child to remove
  void RemoveChild(unsigned index);

  bool Debug();

  bool Debug(bool newval);
};
} // namespace Engine
} // namespace Aspen

#endif
