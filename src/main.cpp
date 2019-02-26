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

  engine.FindChildOfType<Time::Time>()->TargetFramerate(60);

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

  Graphics::Animation *anim = new Graphics::Animation(new Graphics::UniformSpritesheet("resources/anim.png", 4, gfx), 0.16f, gfx);
  anim->FindChildOfType<Transform::Transform>()->SetPosition(50, 0);
  
  Graphics::Line *line = new Graphics::Line(SDL_Point{0, 0}, SDL_Point{50, 0}, 0.5f, 0x0000FFFF, gfx);
  line->FindChildOfType<Transform::Transform>()->SetPosition(100, 100);
  gfx->AddChild(line);
  Graphics::Line *line2 = new Graphics::Line(SDL_Point{0, 0}, SDL_Point{50, 0}, 0.0f, 0xFF0000FF, gfx);
  line2->FindChildOfType<Transform::Transform>()->SetPosition(25, 0);
  line->AddChild(line2);
  line2->AddChild(anim);

  while (engine)
  {
    double dt = M_PI_2 * engine.FindChildOfType<Time::Time>()->CurrentTime();
    line->FindChildOfType<Transform::Transform>()
        ->SetRotation(dt);
    line2->FindChildOfType<Transform::Transform>()
        ->SetRotation(2.0 * dt);

    engine();
  }
  return 0;
}
