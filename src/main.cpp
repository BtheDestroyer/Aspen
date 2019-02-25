#include "Engine.hpp"
#include "Transform.hpp"
#include "Log.hpp"

using namespace Aspen;

int main(int argc, char **argv)
{
  Log::Log::SetFile("./Aspen.log");

  Engine::Engine engine(
      Engine::START_FLAGS::CREATE_GRAPHICS |
      Engine::START_FLAGS::CREATE_EVENTHANDLER |
      Engine::START_FLAGS::CREATE_EVENT_ALL |
      Engine::START_FLAGS::CREATE_TIME);

  //engine.FindChildOfType<Graphics::Graphics>()->AddChild(new Graphics::Sprite("resources/hello_world.bmp", engine.FindChildOfType<Graphics::Graphics>()));
  Graphics::Sprite *newSprite = new Graphics::Sprite("resources/mario.png", engine.FindChildOfType<Graphics::Graphics>());
  newSprite->FindChildOfType<Transform::Transform>()->SetPosition(100, 50);
  engine.FindChildOfType<Graphics::Graphics>()->AddChild(newSprite);

  engine.PrintTree();

  while (engine)
    engine();
  return 0;
}
