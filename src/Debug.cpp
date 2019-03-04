#define __DEBUG_CPP

#include "Debug.hpp"
#include "Time.hpp"
#include "Engine.hpp"
#include "Input.hpp"
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
  if (Active())
    Object::operator()();
  else
    return;

  Engine::Engine *engine = FindAncestorOfType<Engine::Engine>();
  if (engine && engine->Debug())
  {
    //TODO: Get input from an Input wrapper
    Input::Mouse mouse = Input::GetMouse();
    Time::Time *time = engine->FindChildOfType<Time::Time>();
    if (time)
      _io->DeltaTime = std::max(0.000001, time->DeltaTime());
    else
      _io->DeltaTime = 1.0f / 60.0f;
    _io->MousePos = ImVec2(static_cast<float>(mouse.x), static_cast<float>(mouse.y));
    _io->MouseWheel = mouse.wheel;
    _io->MouseDown[0] = mouse.left.held;
    _io->MouseDown[1] = mouse.right.held;

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
  sprintf(buffer, "%s (%p)", o->Name().c_str(), o);
  if (ImGui::TreeNode(buffer))
  {
    unsigned i = 0;
    Object *child = (*o)[i];
    ImGui::Text(o->Active() ? "Active" : "Inactive");
    sprintf(buffer, "Activate##%p", o);
    if (ImGui::Button(buffer))
      o->Activate();
    sprintf(buffer, "Deactivate##%p", o);
    if (ImGui::Button(buffer))
      o->Deactivate();
    sprintf(buffer, "End##%p", o);
    if (ImGui::Button(buffer))
      o->End();
    o->PopulateDebugger();
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
