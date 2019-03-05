#include "Engine.hpp"
#include "Graphics.hpp"
#include "Transform.hpp"
#include "Controller.hpp"
#include "Physics.hpp"
#include "Time.hpp"
#include "Log.hpp"
#include "Audio.hpp"
#include "Input.hpp"
#include "GameState.hpp"
#include <cmath>

using namespace Aspen;

class MyState : public GameState::GameState
{
  Graphics::Line *line = nullptr;
  Graphics::Line *line2 = nullptr;
  Audio::SoundEffect *sound = nullptr;

public:
  MyState(Object *parent = nullptr, std::string name = "MyState")
      : GameState(parent, name)
  {
    Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();

    Graphics::Graphics *gfx = engine->FindChildOfType<Graphics::Graphics>();
    Graphics::FontCache *fc = gfx->FindChildOfType<Graphics::FontCache>();
    fc->LoadFont("resources/ABeeZee-Regular.ttf", "abz");

    Graphics::Sprite *newSprite = new Graphics::Sprite("resources/hello_world.bmp", this);
    int w, h;
    SDL_GetWindowSize(gfx->GetWindow(), &w, &h);
    newSprite->FindChildOfType<Transform::Transform>()->SetPosition(w / 2, h / 2);
    AddChild(newSprite);

    newSprite = new Graphics::Sprite("resources/mario.png", this);
    newSprite->FindChildOfType<Transform::Transform>()->SetPosition(200, 200);
    newSprite->CreateChild<Physics::Rigidbody>();
    Controller::PlayerController_Sidescroller *pc = newSprite->CreateChild<Controller::PlayerController_Sidescroller>();
    //Controller::PlayerController_8Way *pc = newSprite->CreateChild<Controller::PlayerController_8Way>();
    pc->SetJumpStrength(5.0);
    pc->SetJumpHeight(0.25);
    pc->SetAcceleration(5.0);
    pc->SetSpeed(4.0);
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

    Graphics::Text *text = new Graphics::Text("Hello world!", "abz", 36, this);
    text->CreateChild<Transform::Transform>()->SetPosition(w / 2, h / 2);
    AddChild(text);

    Audio::Music *music = new Audio::Music("resources/mus.ogg", this);
    AddChild(music);
    music->Play(true, 2.0);

    sound = new Audio::SoundEffect("resources/pop.wav", this);
    AddChild(sound);
  }

  void OnUpdate()
  {
    Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();
    double dt = M_PI_2 * engine->FindChildOfType<Time::Time>()->CurrentTime();
    line->FindChildOfType<Transform::Transform>()->SetRotation(dt);
    line2->FindChildOfType<Transform::Transform>()->SetRotation(2.0 * dt);
    if (Input::KeyPressed(SDLK_p))
      sound->Play();
  }
};

int main(int argc, char **argv)
{
  Log::Log::SetFile("./Aspen.log");

  Engine::Engine engine(Engine::START_FLAGS::ALL);
  engine.FindChildOfType<Physics::Physics>()->SetGravityStrength(1);
  engine.FindChildOfType<Physics::Physics>()->SetDrag(0.1);
  engine.FindChildOfType<Time::Time>()->TargetFramerate(60);

  engine.FindChildOfType<GameState::GameStateManager>()->LoadState<MyState>(true);

  while (engine)
    engine();
  return 0;
}
