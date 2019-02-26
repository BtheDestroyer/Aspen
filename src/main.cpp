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
      Engine::START_FLAGS::ALL);

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

  gfx->AddChild(new Graphics::Rectangle(SDL_Rect{50, 100, 25, 75}, 0xFF000088, gfx));
  gfx->AddChild(new Graphics::Point(SDL_Point{75, 25}, 0x660099FF, gfx));
  Graphics::Line *line = new Graphics::Line(SDL_Point{100, 100}, SDL_Point{150, 50}, 0x0000FFFF, gfx);
  gfx->AddChild(line);

  //engine.PrintTree();

  while (engine)
  {
    double dt = engine.FindChildOfType<Time::Time>()->CurrentTime();
    line->FindChildOfType<Transform::Transform>()
        ->SetRotation(dt);

    engine();
  }
  return 0;
}
