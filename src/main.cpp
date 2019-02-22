#include "Engine.hpp"
#include "Log.hpp"

using namespace Aspen;

int main(int argc, char **argv)
{
  Log::Log::SetFile("./Aspen.log");

  Engine::Engine engine(
      Engine::START_FLAGS::CREATE_GRAPHICS |
      Engine::START_FLAGS::CREATE_EVENTHANDLER |
      Engine::START_FLAGS::CREATE_EVENT_ALL);

  engine.Graphics()->AddChild(new Graphics::Sprite("resources/hello_world.bmp", engine.Graphics()));
  engine.Graphics()->AddChild(new Graphics::Sprite("resources/mario.png", engine.Graphics()));

  engine.PrintTree();
  
  while (engine)
    engine();
  return 0;
}
