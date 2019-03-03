#define __TRANSFORM_CPP

#include "Transform.hpp"
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
  float cx = 0;
  float cy = 0;
  std::vector<Transform *> tfs;
  Object *p = _parent;
  while (p)
  {
    Transform *tf = dynamic_cast<Transform *>(p);
    if (!tf)
      tf = p->FindChildOfType<Transform>();
    if (tf && tf != this)
      tfs.push_back(tf);
    p = p->Parent();
  }
  for (unsigned i = tfs.size(); i > 0; --i)
  {
    Transform *tf = tfs[i - 1];
    if (i < tfs.size())
    {
      float tx = cx + tf->_posx * std::cos(tfs[i]->GetRotation()) - tf->_posy * std::sin(tfs[i]->GetRotation());
      float ty = cy + tf->_posx * std::sin(tfs[i]->GetRotation()) + tf->_posy * std::cos(tfs[i]->GetRotation());
      cx = tx;
      cy = ty;
    }
    else
    {
      cx = tf->_posx;
      cy = tf->_posy;
    }
  }
  if (tfs.size() > 0)
  {
    float tx = cx + _posx * std::cos(tfs[0]->GetRotation()) - _posy * std::sin(tfs[0]->GetRotation());
    cx = tx;
  }
  else
  {
    cx = _posx;
  }
  return cx;
}

float Transform::GetYPosition() const
{
  float cx = 0;
  float cy = 0;
  std::vector<Transform *> tfs;
  Object *p = _parent;
  while (p)
  {
    Transform *tf = dynamic_cast<Transform *>(p);
    if (!tf)
      tf = p->FindChildOfType<Transform>();
    if (tf && tf != this)
      tfs.push_back(tf);
    p = p->Parent();
  }
  for (unsigned i = tfs.size(); i > 0; --i)
  {
    Transform *tf = tfs[i - 1];
    if (i < tfs.size())
    {
      float tx = cx + tf->_posx * std::cos(tfs[i]->GetRotation()) - tf->_posy * std::sin(tfs[i]->GetRotation());
      float ty = cy + tf->_posx * std::sin(tfs[i]->GetRotation()) + tf->_posy * std::cos(tfs[i]->GetRotation());
      cx = tx;
      cy = ty;
    }
    else
    {
      cx = tf->_posx;
      cy = tf->_posy;
    }
  }
  if (tfs.size() > 0)
  {
    float ty = cy + _posx * std::sin(tfs[0]->GetRotation()) + _posy * std::cos(tfs[0]->GetRotation());
    cy = ty;
  }
  else
  {
    cy = _posy;
  }
  return cy;
}

double Transform::GetRotation() const
{
  double r = _r;
  Object *p = _parent;
  while (p)
  {
    Transform *tf = dynamic_cast<Transform *>(p);
    if (!tf)
      tf = p->FindChildOfType<Transform>();
    if (tf && tf != this)
      r += tf->_r;
    p = p->Parent();
  }
  return std::fmod(r, 360.0);
}

float Transform::GetXScale() const
{
  float r = _scalex;
  Object *p = _parent;
  while (p)
  {
    Transform *tf = dynamic_cast<Transform *>(p);
    if (!tf)
      tf = p->FindChildOfType<Transform>();
    if (tf && tf != this)
      r *= tf->_scalex;
    p = p->Parent();
  }
  return r;
}

float Transform::GetYScale() const
{
  float r = _scaley;
  Object *p = _parent;
  while (p)
  {
    Transform *tf = dynamic_cast<Transform *>(p);
    if (!tf)
      tf = p->FindChildOfType<Transform>();
    if (tf && tf != this)
      r *= tf->_scaley;
    p = p->Parent();
  }
  return r;
}

Transform Transform::operator+(const Transform &rhs)
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
  ImGui::Text("X Pos: %f (%f)", _posx, GetXPosition());
  ImGui::Text("Y Pos: %f (%f)", _posy, GetYPosition());
  ImGui::Text("Rotation: %f (%f)", _r, GetRotation());
  ImGui::Text("X Scale: %f (%f)", _scalex, GetXScale());
  ImGui::Text("Y Scale: %f (%f)", _scaley, GetYScale());
  Object::PopulateDebugger();
}
} // namespace Transform
} // namespace Aspen
