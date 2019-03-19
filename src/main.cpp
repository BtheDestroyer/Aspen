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
  Audio::SoundEffect *sound = nullptr;
  Graphics::Camera *cam;

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

    Graphics::Rectangle *rec = new Graphics::Rectangle(SDL_Rect{0, 0, 50, 50}, 0x0000FFFF, true, this, "Bumper");
    rec->AddChild(new Physics::Rigidbody());
    //rec->AddChild(new Physics::CircleCollider(25, rec));
    rec->AddChild(new Physics::AABBCollider(50, 50, rec));
    rec->FindChildOfType<Transform::Transform>()->SetPosition(500, 200);
    AddChild(rec);

    rec = new Graphics::Rectangle(SDL_Rect{0, 0, 25, 75}, 0xFF000088, true, this);
    rec->FindChildOfType<Transform::Transform>()->SetPosition(25,100);
    AddChild(rec);
    AddChild(new Graphics::Point(SDL_Point{75, 25}, 0x660099FF, this));

    Graphics::Animation *anim = new Graphics::Animation(new Graphics::UniformSpritesheet("resources/anim.png", 4, this), 0.16f, this);
    anim->FindChildOfType<Transform::Transform>()->SetPosition(50, 0);
    anim->FindChildOfType<Transform::Transform>()->SetPosition(200, 200);
    anim->CreateChild<Physics::Rigidbody>();
    //Controller::PlayerController_Sidescroller *pc = newSprite->CreateChild<Controller::PlayerController_Sidescroller>();
    Controller::PlayerController_8Way *pc = anim->CreateChild<Controller::PlayerController_8Way>();
    //pc->SetJumpStrength(5.0);
    //pc->SetJumpHeight(0.25);
    pc->SetAcceleration(5.0);
    pc->SetSpeed(4.0);
    //anim->AddChild(new Physics::CircleCollider(50, this));
    anim->AddChild(new Physics::AABBCollider(50, 50, rec));
    AddChild(anim);

    Graphics::Text *text = new Graphics::Text("Hello world!", "abz", 36, this);
    text->CreateChild<Transform::Transform>()->SetPosition(w / 2, h / 2);
    AddChild(text);

    Audio::Music *music = new Audio::Music("resources/mus.ogg", this);
    AddChild(music);
    music->Play(true, 2.0);

    sound = new Audio::SoundEffect("resources/pop.wav", this);
    AddChild(sound);

    cam = CreateChild<Graphics::Camera>();
    cam->FindChildOfType<Transform::Transform>()->SetPosition(100, -20);
    cam->SelectCamera();
  }

  void OnUpdate()
  {
    if (Input::KeyHeld(SDLK_LEFT))
    {
      if (Input::KeyHeld(SDLK_LSHIFT))
        cam->FindChildOfType<Transform::Transform>()->ModifyXScale(0.99);
      else
        cam->FindChildOfType<Transform::Transform>()->ModifyXPosition(-10);
    }
    if (Input::KeyHeld(SDLK_RIGHT))
    {
      if (Input::KeyHeld(SDLK_LSHIFT))
        cam->FindChildOfType<Transform::Transform>()->ModifyXScale(1.01);
      else
        cam->FindChildOfType<Transform::Transform>()->ModifyXPosition(10);
    }
    if (Input::KeyHeld(SDLK_UP))
    {
      if (Input::KeyHeld(SDLK_LSHIFT))
        cam->FindChildOfType<Transform::Transform>()->ModifyYScale(1.01);
      else
        cam->FindChildOfType<Transform::Transform>()->ModifyYPosition(-10);
    }
    if (Input::KeyHeld(SDLK_DOWN))
    {
      if (Input::KeyHeld(SDLK_LSHIFT))
        cam->FindChildOfType<Transform::Transform>()->ModifyYScale(0.99);
      else
        cam->FindChildOfType<Transform::Transform>()->ModifyYPosition(10);
    }

    if (Input::KeyHeld(SDLK_e))
      cam->FindChildOfType<Transform::Transform>()->ModifyRotation(0.01 * 2 * M_PI);
    if (Input::KeyHeld(SDLK_q))
      cam->FindChildOfType<Transform::Transform>()->ModifyRotation(-0.01 * 2 * M_PI);

    if (Input::KeyPressed(SDLK_p))
      sound->Play();
  }
};

int main(int argc, char **argv)
{
  Log::Log::SetFile("./Aspen.log");

  Engine::Engine engine(Engine::START_FLAGS::ALL);
  engine.FindChildOfType<Physics::Physics>()->SetGravityStrength(0);
  engine.FindChildOfType<Physics::Physics>()->SetDrag(0.1);
  engine.FindChildOfType<Time::Time>()->TargetFramerate(60);

  engine.FindChildOfType<GameState::GameStateManager>()->LoadState<MyState>(true);

  while (engine)
    engine();
  return 0;
}
