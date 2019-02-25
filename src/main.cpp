#include "Engine.hpp"
#include "Transform.hpp"
#include "Controller.hpp"
#include "Log.hpp"
#include <math.h>

using namespace Aspen;

int main(int argc, char **argv)
{
  Log::Log::SetFile("./Aspen.log");

  Engine::Engine engine(
      Engine::START_FLAGS::CREATE_GRAPHICS |
      Engine::START_FLAGS::CREATE_EVENTHANDLER |
      Engine::START_FLAGS::CREATE_EVENT_ALL |
      Engine::START_FLAGS::CREATE_TIME);

  Graphics::Graphics *gfx = engine.FindChildOfType<Graphics::Graphics>();

  Graphics::Sprite *newSprite = new Graphics::Sprite("resources/hello_world.bmp", gfx);
  gfx->AddChild(new Graphics::Sprite("resources/hello_world.bmp", gfx));
  int w, h;
  SDL_GetWindowSize(gfx->GetWindow(), &w, &h);

  newSprite = new Graphics::Sprite("resources/mario.png", engine.FindChildOfType<Graphics::Graphics>());
  newSprite->FindChildOfType<Transform::Transform>()->SetPosition(200, 200);
  //newSprite->FindChildOfType<Transform::Transform>()->SetScale(0.5, 1);
  //newSprite->FindChildOfType<Transform::Transform>()->SetRotation(10.0);
  newSprite->AddChild(new Controller::PlayerController_8Way());
  gfx->AddChild(newSprite);

  engine.PrintTree();

  while (engine)
  {
    engine();
  }
  return 0;
}
