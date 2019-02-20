#ifndef __ENGINE_HPP
#define __ENGINE_HPP
#include "Graphics.hpp"
#include "Event.hpp"

namespace Gladius
{
namespace Engine
{
namespace START_FLAGS
{
const int NONE                       = 0b0000000000000000;
const int CREATE_GRAPHICS            = 0b0000000000000001;
const int CREATE_EVENTHANDLER        = 0b0000000000000010;
const int CREATE_EVENT_QUIT          = 0b0000000000000100;
const int CREATE_EVENT_KEYS_ALPHA    = 0b0000000000001000;
const int CREATE_EVENT_KEYS_NUM      = 0b0000000000010000;
const int CREATE_EVENT_KEYS_SPECIAL  = 0b0000000000100000;
const int CREATE_EVENT_KEYS_FUNCTION = 0b0000000001000000;
const int CREATE_EVENT_KEYS_ALL      = 0b0000000001111000;
const int CREATE_EVENT_ALL           = 0b0000000001111100;
const int ALL                        = 0b1111111111111111;
} // namespace START_FLAGS

class Engine : public Object::Object
{
  Graphics::Graphics *_graphics;
  Event::EventHandler *_eventhandler;

public:
  Engine(int flags = START_FLAGS::NONE);
  ~Engine();

  void RefreshGraphics();
  Graphics::Graphics *Graphics();
  void RefreshEventHandler();
  Event::EventHandler *EventHandler();

  void RemoveChild(Object *child);
  void RemoveChild(int index);
};
} // namespace Engine
} // namespace Gladius

#endif
