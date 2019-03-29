#define __TRANSFORM_CPP

#include "Transform.hpp"
#include "Engine.hpp"
#include "Graphics.hpp"
#include <cmath>
#include "imgui.h"

#undef __TRANSFORM_CPP

namespace Aspen
{
namespace Transform
{
Transform::Transform(Object *parent, std::string name)
    : Object(parent, name), _posx(0), _posy(0), _r(0), _scalex(1), _scaley(1)
{
}

void Transform::SetPosition(float x, float y)
{
  _posx = x;
  _posy = y;
}

void Transform::SetXPosition(float x)
{
  _posx = x;
}

void Transform::SetYPosition(float y)
{
  _posy = y;
}

void Transform::SetRotation(double r)
{
  _r = std::fmod(r, 360.0);
}

void Transform::SetScale(float x, float y)
{
  _scalex = x;
  _scaley = y;
}

void Transform::SetXScale(float x)
{
  _scalex = x;
}

void Transform::SetYScale(float y)
{
  _scaley = y;
}

void Transform::ModifyPosition(float x, float y)
{
  _posx += x;
  _posy += y;
}

void Transform::ModifyXPosition(float x)
{
  _posx += x;
}

void Transform::ModifyYPosition(float y)
{
  _posy += y;
}

void Transform::ModifyRotation(double r)
{
  _r = std::fmod(_r + r, 360.0);
}

void Transform::ModifyScale(float x, float y)
{
  _scalex *= x;
  _scaley *= y;
}

void Transform::ModifyXScale(float x)
{
  _scalex *= x;
}

void Transform::ModifyYScale(float y)
{
  _scaley *= y;
}

float Transform::GetXPosition() const
{
  std::vector<const Transform *> tfs;
  const Object *p = this;
  tfs.push_back(this);
  while (p)
  {
    const Transform *tf = dynamic_cast<const Transform *>(p);
    if (!tf)
      tf = p->GetTransform();
    if (tf && tf != this)
      tfs.push_back(tf);
    p = p->Parent();
  }
  float ret = 0.0f;
  for (const Transform *tf : tfs)
  {
    // TODO: Incorporate rotation
    ret += tf->GetLocalXPosition() * tf->GetXScale() * tf->GetLocalInverseXScale();
  }
  return ret;
}

float Transform::GetYPosition() const
{
  std::vector<const Transform *> tfs;
  const Object *p = this;
  tfs.push_back(this);
  while (p)
  {
    const Transform *tf = dynamic_cast<const Transform *>(p);
    if (!tf)
      tf = p->GetTransform();
    if (tf && tf != this)
      tfs.push_back(tf);
    p = p->Parent();
  }
  float ret = 0.0f;
  for (const Transform *tf : tfs)
  {
    // TODO: Incorporate rotation
    ret += tf->GetLocalYPosition() * tf->GetYScale() * tf->GetLocalInverseYScale();
  }
  return ret;
}

double Transform::GetRotation() const
{
  const Object *p = this;
  double ret = 0.0;
  while (p)
  {
    const Transform *tf = dynamic_cast<const Transform *>(p);
    if (!tf)
      tf = p->GetTransform();
    if (tf)
      ret += tf->GetLocalRotation();
    p = p->Parent();
  }
  return ret;
}

float Transform::GetXScale() const
{
  const Object *p = this;
  double ret = 1.0;
  while (p)
  {
    const Transform *tf = dynamic_cast<const Transform *>(p);
    if (!tf)
      tf = p->GetTransform();
    if (tf)
      ret *= tf->GetLocalXScale();
    p = p->Parent();
  }
  return ret;
}

float Transform::GetYScale() const
{
  const Object *p = this;
  double ret = 1.0;
  while (p)
  {
    const Transform *tf = dynamic_cast<const Transform *>(p);
    if (!tf)
      tf = p->GetTransform();
    if (tf)
      ret *= tf->GetLocalYScale();
    p = p->Parent();
  }
  return ret;
}

float Transform::GetXPosition(const Transform *camera) const
{
  if (!camera)
    return GetXPosition();
  int w, h;
  Engine::Engine *e = Engine::Engine::Get();
  if (!e)
    return GetXPosition() + camera->GetInverseXPosition();
  Graphics::Graphics *g = e->FindChildOfType<Graphics::Graphics>();
  if (!g)
    return GetXPosition() + camera->GetInverseXPosition();
  SDL_GetWindowSize(g->GetWindow(), &w, &h);
  return ((GetXPosition() + camera->GetInverseXPosition()) - w / 2.0f) * camera->GetInverseXScale() + w / 2.0f;
}

float Transform::GetYPosition(const Transform *camera) const
{
  if (!camera)
    return GetYPosition();
  int w, h;
  Engine::Engine *e = Engine::Engine::Get();
  if (!e)
    return GetYPosition() + camera->GetInverseYPosition();
  Graphics::Graphics *g = e->FindChildOfType<Graphics::Graphics>();
  if (!g)
    return GetYPosition() + camera->GetInverseYPosition();
  SDL_GetWindowSize(g->GetWindow(), &w, &h);
  return ((GetYPosition() + camera->GetInverseYPosition()) - h / 2.0f) * camera->GetInverseYScale() + h / 2.0f;
}

double Transform::GetRotation(const Transform *camera) const
{
  if (!camera)
    return GetRotation();
  return GetRotation() + camera->GetInverseRotation();
}

float Transform::GetXScale(const Transform *camera) const
{
  if (!camera)
    return GetXScale();
  return GetXScale() * camera->GetInverseXScale();
}

float Transform::GetYScale(const Transform *camera) const
{
  if (!camera)
    return GetYScale();
  return GetYScale() * camera->GetInverseYScale();
}

float Transform::GetInverseXPosition() const
{
  return -GetXPosition();
}

float Transform::GetInverseYPosition() const
{
  return -GetYPosition();
}

double Transform::GetInverseRotation() const
{
  return -GetRotation();
}

float Transform::GetInverseXScale() const
{
  return 1.0f / GetXScale();
}

float Transform::GetInverseYScale() const
{
  return 1.0f / GetYScale();
}

float Transform::GetLocalXPosition() const
{
  return _posx;
}

float Transform::GetLocalYPosition() const
{
  return _posy;
}

double Transform::GetLocalRotation() const
{
  return _r;
}

float Transform::GetLocalXScale() const
{
  return _scalex;
}

float Transform::GetLocalYScale() const
{
  return _scaley;
}

float Transform::GetLocalInverseXPosition() const
{
  return -GetLocalXPosition();
}

float Transform::GetLocalInverseYPosition() const
{
  return -GetLocalYPosition();
}

double Transform::GetLocalInverseRotation() const
{
  return -GetLocalRotation();
}

float Transform::GetLocalInverseXScale() const
{
  return 1.0f / GetLocalXScale();
}

float Transform::GetLocalInverseYScale() const
{
  return 1.0f / GetLocalYScale();
}

Transform Transform::Inverse() const
{
  Transform tf;

  tf._posx = GetLocalInverseXPosition();
  tf._posy = GetLocalInverseYPosition();
  tf._r = GetLocalInverseRotation();
  tf._scalex = GetLocalInverseXScale();
  tf._scaley = GetLocalInverseYScale();
  return tf;
}

Transform Transform::operator+(const Transform &rhs) const
{
  Transform tf = *this;
  return tf += rhs;
}

Transform &Transform::operator+=(const Transform &rhs)
{
  ModifyPosition(rhs.GetXPosition(), rhs.GetYPosition());
  ModifyRotation(rhs.GetRotation());
  ModifyScale(rhs.GetXScale(), rhs.GetYScale());
  return *this;
}

void Transform::PopulateDebugger()
{
  ImGui::DragFloat2("Pos", &_posx, 1.0f);
  static float r = 0;
  r = float(_r);
  ImGui::DragFloat("Rotation", &r, M_PI / 180.0f);
  if (std::abs(r - _r) >= M_PI / 180.0f)
    _r = r;
  ImGui::DragFloat2("Scale", &_scalex, 0.01f);
  Object::PopulateDebugger();
}
} // namespace Transform
} // namespace Aspen
