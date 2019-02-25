#define __TRANSFORM_CPP

#include "Transform.hpp"

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

void Transform::SetRotation(float r)
{
  _r = r;
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

void Transform::ModifyRotation(float r)
{
  _r += r;
  while (_r > 360.0f)
    _r -= 360.0f;
  while (_r < 0.0f)
    _r += 360.0f;
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
  float r = _posx;
  Object *p = _parent;
  while (p)
  {
    Transform *tf = dynamic_cast<Transform *>(p);
    if (!tf)
      tf = p->FindChildOfType<Transform>();
    if (tf && tf != this)
      r += tf->_posx;
    p = p->Parent();
  }
  return r;
}

float Transform::GetYPosition() const
{
  float r = _posy;
  Object *p = _parent;
  while (p)
  {
    Transform *tf = dynamic_cast<Transform *>(p);
    if (!tf)
      tf = p->FindChildOfType<Transform>();
    if (tf && tf != this)
      r += tf->_posy;
    p = p->Parent();
  }
  return r;
}

float Transform::GetRotation() const
{
  float r = _r;
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
  while (r > 360.0f)
    r -= 360.0f;
  while (r < 0.0f)
    r += 360.0f;
  return r;
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
} // namespace Transform
} // namespace Aspen
