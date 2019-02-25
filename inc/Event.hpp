#ifndef __EVENT_HPP
#define __EVENT_HPP
#include "Object.hpp"
#include <SDL2/SDL.h>

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Event namespace
///        Contains EventListeners and EventHandler Objects
namespace Event
{
/// \brief EventListener class
///        Parented by EventHandler and used to moduly handle SDL_Events
class EventListener : public Object::Object
{
public:
  /// \brief Constructor
  /// \param name Object name (Same as type)
  EventListener(Object *parent = nullptr, std::string name = "EventListener");
  /// \brief Destructor
  ~EventListener();

  /// \brief Functions like Object::operator() if event != nullptr
  ///        Calls Handle(event) if event != nullptr
  ///        This is called twice (once with an event, once without) by a parent EventHandler
  /// \param event Event to handle
  void operator()(SDL_Event *event = nullptr);

  /// \brief Handles event passed by operator()
  ///        Redefined by inheriting classes
  /// \param event Event to handle
  virtual void Handle(SDL_Event *event);
};

/// \brief QuitEventListener class
class QuitEventListener : public EventListener
{
public:
  /// \brief Constructor
  QuitEventListener(Object *parent = nullptr, std::string name = "QuitEventListener");
  /// \brief Destructor
  ~QuitEventListener();

  /// \brief Calls Root()->End() to shutdown the entire Object tree if passed an SDL_QUIT event
  /// \param event Event to handle
  void Handle(SDL_Event *event);
};

/// \brief KeyEventListener class
///        Controls Input::Keys from Input::GetKey()
class KeyEventListener : public EventListener
{
  /// \brief Key to pass Input::GetKey()
  SDL_Keycode _key;

public:
  /// \brief Constructor
  /// \param k Which Key's states to control
  KeyEventListener(Object *parent = nullptr, std::string name = "EventListener");
  KeyEventListener(SDL_Keycode k = SDLK_UNKNOWN, Object *parent = nullptr, std::string name = "KeyEventListener");
  /// \brief Destructor
  ~KeyEventListener();

  /// \brief Sets _key
  /// \param k Key to control
  void SetKey(SDL_Keycode k);
  /// \brief Controls the state of Input::Key in the map obtained by Input::GetKey(_key)
  ///        Does nothing if _key == SDLK_UNKNOWN
  /// \param event Event to handle
  void Handle(SDL_Event *event);
};

/// \brief EventHandler class
///        Passes SDL_Events to children EventListeners
class EventHandler : public Object::Object
{
public:
  /// \brief Constructor
  EventHandler(Object *parent = nullptr, std::string name = "EventHandler");
  /// \brief Destructor
  ~EventHandler();
  /// \brief Polls SDL_Events and passes them to all child EventListeners
  void operator()();
};
} // namespace Event
} // namespace Aspen

#endif
