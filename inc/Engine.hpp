#ifndef __ENGINE_HPP
#define __ENGINE_HPP
#include "Version.hpp"
#include "Object.hpp"

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
/// \brief Creates a Debug::Debug Object as a child of created Graphics::Graphics
///        Must be passed with CREATE_GRAPHICS
const int CREATE_GRAPHICS_DEBUGGER   = 0b0000000000000010;
/// \brief Creates an Event::EventHandler Object as a child
const int CREATE_EVENTHANDLER        = 0b0000000000000100;
/// \brief Gives the child EventHandler a QuitEventListener
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_QUIT          = 0b0000000000001000;
/// \brief Gives the child EventHandler a KeyEventListener for each alphabet key
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_KEYS_ALPHA    = 0b0000000000010000;
/// \brief Gives the child EventHandler a KeyEventListener for each numeric key
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_KEYS_NUM      = 0b0000000000100000;
/// \brief Gives the child EventHandler a KeyEventListener for each special key
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_KEYS_SPECIAL  = 0b0000000001000000;
/// \brief Gives the child EventHandler a KeyEventListener for each function key
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_KEYS_FUNCTION = 0b0000000010000000;
/// \brief Synonym for (CREATE_EVENT_KEYS_ALPHA | CREATE_EVENT_KEYS_NUM | CREATE_EVENT_KEYS_SPECIAL | CREATE_EVENT_KEYS_FUNCTION | CREATE_EVENT_KEYS_ALL)
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_KEYS_ALL      = 0b0000000011110000;
/// \brief Synonym for (CREATE_EVENT_QUIT | CREATE_EVENT_KEYS_ALL)
///        Must be passed with CREATE_EVENTHANDLER
const int CREATE_EVENT_ALL           = 0b0000000011111000;
/// \brief Creates an Time::Time Object as a child
const int CREATE_TIME                = 0b0000000100000000;
/// \brief Creates an Physics::Physics Object as a child
const int CREATE_PHYSICS             = 0b0000001000000000;
/// \brief Determines if children of the engine should debug
const int DEBUGGING_ON               = 0b1000000000000000;
/// \brief Synonym for all START_FLAGS
const int ALL                        = 0b1111111111111111;
} // namespace START_FLAGS

/// \brief Engine class
class Engine : public Object::Object
{
  bool _debugging = false;

public:
  /// \brief Constructor
  /// \param flags START_FLAGS to instantiate the Engine with
  Engine(Object *parent = nullptr, std::string name = "Engine");
  Engine(int flags = START_FLAGS::NONE, Object *parent = nullptr, std::string name = "Engine");
  /// \brief Destructor
  ~Engine();

  bool Debug();
  void Debug(bool newval);

  void PopulateDebugger();
};
} // namespace Engine
} // namespace Aspen

#endif
