#define __DEBUG_CPP

#include "Debug.hpp"
#include "Time.hpp"
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
  if (Valid())
    Object::operator()();
  else
    return;

  Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();
  if (engine && engine->Debug())
  {
    //TODO: Get input from an Input wrapper
    int mouseX,
        mouseY;
    const int buttons = SDL_GetMouseState(&mouseX, &mouseY);
    Time::Time *time = engine->FindChildOfType<Time::Time>();
    if (time)
      _io->DeltaTime = std::max(0.000001, time->DeltaTime());
    else
      _io->DeltaTime = 1.0f / 60.0f;
    _io->MousePos = ImVec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
    _io->MouseDown[0] = buttons & SDL_BUTTON(SDL_BUTTON_LEFT);
    _io->MouseDown[1] = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);

    ImGui::NewFrame();
    ImGui::Begin("Object Tree", NULL, ImVec2(400, 400));
    char buffer[256];
    if (time)
      sprintf(buffer, "FPS: %f", time->FPS());
    else
      sprintf(buffer, "FPS: ???");
    ImGui::Text(buffer);
    MakeTree(Root());
    ImGui::End();
    ImGui::Render();
    ImGuiSDL::Render(ImGui::GetDrawData());
  }
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
    o->PopulateDebugger();
    if (o->Valid() && ImGui::Button(buffer))
      o->End();
    while (child)
    {
      MakeTree(child);
      child = (*o)[++i];
    }
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

void Debug::PopulateDebugger()
{
  ImGui::Text("ImGuiIO: %p", _io);
  ImGui::Text("Debugger Count: %d", _dcount);
  Object::PopulateDebugger();
}
} // namespace Debug
} // namespace Aspen
