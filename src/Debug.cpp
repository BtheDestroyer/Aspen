#define __DEBUG_CPP

#include "Debug.hpp"
#include "Engine.hpp"
#include "Graphics.hpp"
#include "imgui_sdl.h"

#undef __DEBUG_CPP

namespace Aspen
{
namespace Debug
{
unsigned Debug::_dcount = 0;

Debug::Debug(Object *parent, std::string name)
    : Object(parent, name)
{
  if (_dcount++ == 0)
    ImGui::CreateContext();
  if (_parent)
    Setup();
}

void Debug::Setup()
{
  Graphics::Graphics *gfx = dynamic_cast<Graphics::Graphics *>(_parent);
  if (gfx)
  {
    int w, h;
    SDL_GetWindowSize(gfx->GetWindow(), &w, &h);
    ImGuiSDL::Initialize(gfx->GetRenderer(), w, h);
    _io = &ImGui::GetIO();
  }
  else
    _io = nullptr;
}

void Debug::operator()()
{
  //TODO: Get input from an Input wrapper
  int mouseX, mouseY;
  const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
  Time::Time *time = FindAncestorOfType<Engine::Engine>()->FindChildOfType<Time::Time>();
  if (time)
    _io->DeltaTime = std::max(0.000001, time->DeltaTime());
  else
    _io->DeltaTime = 1.0f / 60.0f;
  _io->MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
  _io->MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
  _io->MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

  ImGui::NewFrame();
  ImGui::Begin("Object Tree", NULL, ImVec2(400, 400));
  MakeTree(Root());
  ImGui::End();
  ImGui::Render();
  ImGuiSDL::Render(ImGui::GetDrawData());
}

void Debug::MakeTree(Object *o)
{
  char buffer[128];
  if (!o)
    return;
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

Debug::~Debug()
{
  if (--_dcount == 0)
  {
    ImGuiSDL::Deinitialize();
    ImGui::DestroyContext();
  }
}
} // namespace Debug
} // namespace Aspen
