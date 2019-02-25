#include "Engine.hpp"
#include "Transform.hpp"
#include "Controller.hpp"
#include "Log.hpp"
#include <cmath>
#include "imgui.h"
#include "imgui_sdl.h"

using namespace Aspen;

class ImGuiWrapper : public Object::Object
{
public:
  ImGuiWrapper(Object *parent = nullptr, std::string name = "ImGui Wrapper")
      : Object(parent, name)
  {
    ImGui::CreateContext();
    Graphics::Graphics *gfx = dynamic_cast<Graphics::Graphics *>(_parent);
    if (gfx)
    {
      int w, h;
      SDL_GetWindowSize(gfx->GetWindow(), &w, &h);
      ImGuiSDL::Initialize(gfx->GetRenderer(), w, h);
    }
  }

  void operator()()
  {
    ImGuiIO &io = ImGui::GetIO();

    int mouseX, mouseY;
    const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
    Time::Time *time = FindAncestorOfType<Engine::Engine>()->FindChildOfType<Time::Time>();
    if (time)
      io.DeltaTime = std::max(0.000001, time->DeltaTime());
    else
      io.DeltaTime = 1.0f / 60.0f;
    io.MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
    io.MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    io.MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

    ImGui::NewFrame();
    ImGui::Begin("Object Tree", NULL, ImVec2(400, 400));
    MakeTree(Root());
    ImGui::End();
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
  }

  void MakeTree(Object *o)
  {
    char buffer[128];
    sprintf(buffer, "%s (%p) (%s)", o->Name().c_str(), o, o->Valid() ? "Valid" : "Ended");
    if (ImGui::TreeNode(buffer))
    {
      unsigned i = 0;
      Object *child = (*o)[i];
      sprintf(buffer, "End##%p", o);
      if (o->Valid() && ImGui::Button(buffer))
        o->End();
      while (child)
      {
        MakeTree(child);
        child = (*o)[++i];
      }
      if (i == 0)
        ImGui::Text("No children...");
      ImGui::TreePop();
    }
  }

  ~ImGuiWrapper()
  {
    ImGuiSDL::Deinitialize();
    ImGui::DestroyContext();
  }
};

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
  gfx->AddChild(new ImGuiWrapper(gfx));

  //engine.PrintTree();

  while (engine)
    engine();
  return 0;
}
