#ifndef __EVENT_HPP
#define __EVENT_HPP
#include "Object.hpp"
#include "Input.hpp"
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
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
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

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief QuitEventListener class
class QuitEventListener : public EventListener
{
public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  QuitEventListener(Object *parent = nullptr, std::string name = "QuitEventListener");
  /// \brief Destructor
  ~QuitEventListener();

  /// \brief Calls Root()->End() to shutdown the entire Object tree if passed an SDL_QUIT event
  /// \param event Event to handle
  void Handle(SDL_Event *event);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief KeyEventListener class
///        Controls Input::Keys from Input::GetKey()
class KeyEventListener : public EventListener
{
  /// \brief Key to pass Input::GetKey()
  SDL_Keycode _key;

public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  KeyEventListener(Object *parent = nullptr, std::string name = "KeyEventListener");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param k Which Key's states to control
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  KeyEventListener(SDL_Keycode k, Object *parent = nullptr, std::string name = "KeyEventListener");
  /// \brief Destructor
  ~KeyEventListener();

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Sets _key
  /// \param k Key to control
  void SetKey(SDL_Keycode k);
  /// \brief Controls the state of Input::Key in the map obtained by Input::GetKey(_key)
  ///        Does nothing if _key == SDLK_UNKNOWN
  /// \param event Event to handle
  void Handle(SDL_Event *event);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief MouseEventListener class
class MouseEventListener : public EventListener
{
  /// \brief Reference to mouse data
  Input::Mouse &_m;

public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  MouseEventListener(Object *parent = nullptr, std::string name = "MouseEventListener");
  /// \brief Destructor
  ~MouseEventListener();

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Controls the state of Input::Key in the map obtained by Input::GetKey(_key)
  ///        Does nothing if _key == SDLK_UNKNOWN
  /// \param event Event to handle
  void Handle(SDL_Event *event);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief EventHandler class
///        Passes SDL_Events to children EventListeners
class EventHandler : public Object::Object
{
public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  EventHandler(Object *parent = nullptr, std::string name = "EventHandler");
  /// \brief Destructor
  ~EventHandler();

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};
} // namespace Event
} // namespace Aspen

#endif
