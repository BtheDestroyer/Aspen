#include "Engine.hpp"
#include "Graphics.hpp"
#include "Transform.hpp"
#include "Controller.hpp"
#include "Physics.hpp"
#include "Time.hpp"
#include "Log.hpp"
#include "Debug.hpp"
#include "GameState.hpp"
#include <cmath>

using namespace Aspen;

class MyState : public GameState::GameState
{
  Graphics::Line *line = nullptr;
  Graphics::Line *line2 = nullptr;

public:
  MyState(Object *parent = nullptr, std::string name = "MyState")
      : GameState(parent, name)
  {
    Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();

    Graphics::Sprite *newSprite = new Graphics::Sprite("resources/hello_world.bmp", this);
    int w, h;
    SDL_GetWindowSize(engine->FindChildOfType<Graphics::Graphics>()->GetWindow(), &w, &h);
    newSprite->FindChildOfType<Transform::Transform>()->SetPosition(w / 2, h / 2);
    AddChild(newSprite);

    newSprite = new Graphics::Sprite("resources/mario.png", this);
    newSprite->FindChildOfType<Transform::Transform>()->SetPosition(200, 200);
    //newSprite->FindChildOfType<Transform::Transform>()->SetScale(0.5, 1);
    //newSprite->FindChildOfType<Transform::Transform>()->SetRotation(10.0);
    newSprite->CreateChild<Physics::Rigidbody>();
    newSprite->CreateChild<Controller::PlayerController_8Way>();
    AddChild(newSprite);

    AddChild(new Graphics::Rectangle(SDL_Rect{50, 100, 25, 75}, 0xFF000088, true, this));
    AddChild(new Graphics::Point(SDL_Point{75, 25}, 0x660099FF, this));

    line = new Graphics::Line(SDL_Point{0, 0}, SDL_Point{50, 0}, 0.5f, 0x0000FFFF, this);
    line->FindChildOfType<Transform::Transform>()->SetPosition(100, 100);
    AddChild(line);
    line2 = new Graphics::Line(SDL_Point{0, 0}, SDL_Point{50, 0}, 0.0f, 0xFF0000FF, this);
    line2->FindChildOfType<Transform::Transform>()->SetPosition(25, 0);
    line->AddChild(line2);

    Graphics::Animation *anim = new Graphics::Animation(new Graphics::UniformSpritesheet("resources/anim.png", 4, this), 0.16f, this);
    anim->FindChildOfType<Transform::Transform>()->SetPosition(50, 0);
    line2->AddChild(anim);
  }

  void OnUpdate()
  {
    Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();
    double dt = M_PI_2 * engine->FindChildOfType<Time::Time>()->CurrentTime();
    line->FindChildOfType<Transform::Transform>()->SetRotation(dt);
    line2->FindChildOfType<Transform::Transform>()->SetRotation(2.0 * dt);
  }
};

int main(int argc, char **argv)
{
  Log::Log::SetFile("./Aspen.log");

  Engine::Engine engine(
      Engine::START_FLAGS::ALL);
  engine.CreateChild<GameState::GameStateManager>()->LoadState<MyState>(true);

  engine.FindChildOfType<Physics::Physics>()->SetGravityStrength(0);
  engine.FindChildOfType<Time::Time>()->TargetFramerate(60);

  while (engine)
    engine();
  return 0;
}
