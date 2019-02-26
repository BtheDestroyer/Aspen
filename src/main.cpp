#include "Engine.hpp"
#include "Graphics.hpp"
#include "Transform.hpp"
#include "Controller.hpp"
#include "Log.hpp"
#include "Debug.hpp"
#include <cmath>

using namespace Aspen;

int main(int argc, char **argv)
{
  Log::Log::SetFile("./Aspen.log");

  Engine::Engine engine(
      Engine::START_FLAGS::CREATE_GRAPHICS |
      Engine::START_FLAGS::CREATE_GRAPHICS |
      Engine::START_FLAGS::CREATE_EVENTHANDLER |
      Engine::START_FLAGS::CREATE_EVENT_ALL |
      Engine::START_FLAGS::CREATE_TIME |
      Engine::START_FLAGS::DEBUGGING_ON);

  Graphics::Graphics *gfx = engine.FindChildOfType<Graphics::Graphics>();

  Graphics::Sprite *newSprite = new Graphics::Sprite("resources/hello_world.bmp", gfx);
  gfx->AddChild(new Graphics::Sprite("resources/hello_world.bmp", gfx));
  int w, h;
  SDL_GetWindowSize(gfx->GetWindow(), &w, &h);

  newSprite = new Graphics::Sprite("resources/mario.png", gfx);
  newSprite->FindChildOfType<Transform::Transform>()->SetPosition(200, 200);
  //newSprite->FindChildOfType<Transform::Transform>()->SetScale(0.5, 1);
  //newSprite->FindChildOfType<Transform::Transform>()->SetRotation(10.0);
  newSprite->AddChild(new Controller::PlayerController_8Way());
  gfx->AddChild(newSprite);
  gfx->AddChild(new Debug::Debug(gfx));

  //engine.PrintTree();

  while (engine)
    engine();
  return 0;
}
