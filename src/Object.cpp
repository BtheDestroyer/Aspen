#define __OBJECT_CPP

#include "Object.hpp"
#include "Engine.hpp"
#include <algorithm>
#include <iomanip>
#include "imgui.h"

#undef __OBJECT_CPP

namespace Aspen
{
namespace Object
{
int Object::_count = 0;

Object::Object(Object *parent, std::string name)
    : _name(name), _parent(parent)
{
  ++_count;
  if ((dynamic_cast<Engine::Engine *>(this) && dynamic_cast<Engine::Engine *>(this)->Debug()) ||
      (FindAncestorOfType<Engine::Engine>() && FindAncestorOfType<Engine::Engine>()->Debug()))
    Log::Debug("Creating %s:  %p  %d", _name.c_str(), this, _count);

  _valid = true;
}

Object::~Object()
{
  --_count;
  if ((dynamic_cast<Engine::Engine *>(this) && dynamic_cast<Engine::Engine *>(this)->Debug()) ||
      (FindAncestorOfType<Engine::Engine>() && FindAncestorOfType<Engine::Engine>()->Debug()))
  {
    Log::Debug("Destroying %s:  %p  %d", _name.c_str(), this, _count);
    if (_count == 0)
      Log::Debug("All clean :D");
  }
  End();
  for (Object *child : _children)
    delete child;
  _children.clear();
}

const Object *Object::Parent() const
{
  return _parent;
}

Object *Object::Parent()
{
  return _parent;
}

void Object::SetParent(Object *parent)
{
  if (_parent)
    _parent->RemoveChild(this);
  _parent = parent;
}

Object *Object::Root()
{
  Object *root = this;
  while (root->Parent())
    root = root->Parent();
  return root;
}

const Object *Object::Root() const
{
  const Object *root = this;
  while (root->Parent())
    root = root->Parent();
  return root;
}

void Object::operator()()
{
  if (!Active())
    return;
  if (!_started)
  {
    OnStart();
    OnActivate();
    _started = true;
  }
  OnUpdate();
  for (unsigned i = 0; i < _children.size(); ++i)
  {
    Object *child = _children[i];
    (*child)();
    if (!child->Valid())
    {
      RemoveChild(child);
      delete child;
      --i;
    }
  }
}

void Object::AddChild(Object *child)
{
  if (!child || this == child)
    return;
  if (std::find(_children.begin(), _children.end(), child) == _children.end())
  {
    child->SetParent(this);
    _children.push_back(child);
  }
}

void Object::RemoveChild(Object *child)
{
  if (!child || this == child)
    return;
  std::vector<Object *>::iterator it = std::find(_children.begin(), _children.end(), child);
  if (it != _children.end())
  {
    (*it)->_parent = nullptr;
    _children.erase(it);
  }
}

void Object::RemoveChild(unsigned index)
{
  if (index < _children.size())
  {
    _children[index]->_parent = nullptr;
    _children.erase(_children.begin() + index);
  }
}

Object *Object::operator[](unsigned index)
{
  if (index < _children.size())
    return _children[index];
  return nullptr;
}

int Object::operator[](Object *child)
{
  std::vector<Object *>::iterator it = std::find(_children.begin(), _children.end(), child);
  if (it == _children.end())
    return -1;
  return _children.end() - _children.begin();
}

Object *Object::GetLastChild()
{
  if (_children.size() == 0)
    return nullptr;
  return _children[_children.size() - 1];
}

int Object::ParentCount()
{
  int i = 0;
  Object *p = _parent;
  while (p)
  {
    p = p->Parent();
    ++i;
  }
  return i;
}

const bool &Object::Valid() const
{
  return _valid;
}

bool Object::Active() const
{
  return _valid && _active;
}

void Object::SetActive(bool active)
{
  if (active)
    Activate();
  else
    Deactivate();
}

void Object::Activate()
{
  if (!_active)
  {
    _active = true;
    TriggerOnActivate();
  }
}

void Object::Deactivate()
{
  if (_active)
  {
    TriggerOnDeactivate();
    _active = false;
  }
}

void Object::TriggerOnActivate()
{
  if (!Active())
    return;
  if (!_started)
  {
    OnStart();
    _started = true;
  }
  for (Object *c : _children)
    c->TriggerOnActivate();
  OnActivate();
}

void Object::TriggerOnDeactivate()
{
  if (!Active())
    return;
  for (Object *c : _children)
    c->TriggerOnDeactivate();
  OnDeactivate();
}

Object::operator bool() const
{
  return Active();
}

void Object::End()
{
  if (!Valid())
    return;
  OnDeactivate();
  OnEnd();
  for (Object *c : _children)
    c->End();
  _valid = false;
}

void Object::PrintTree(Log::Log &log) const
{
  static std::string indentation = "";
  static const std::string newindent = "  |    ";
  static bool madeSpace = false;
  if (indentation.length() == 0)
  {
    log("%s (%p) (%s)", _name.c_str(), this, Valid() ? "Valid" : "Ended");
    if (_children.size() > 0)
      indentation = "  ";
  }
  else
  {
    if (this == _parent->GetLastChild())
    {
      log("%s\\... %s (%p) (%s)", indentation.c_str(), _name.c_str(), this, Valid() ? "Valid" : "Ended");
      if (_children.size() > 0)
        indentation = indentation + "       ";
    }
    else
    {
      log("%s+--- %s (%p) (%s)", indentation.c_str(), _name.c_str(), this, Valid() ? "Valid" : "Ended");
      if (_children.size() > 0)
        indentation = newindent + indentation;
    }
  }
  madeSpace = false;
  for (unsigned i = 0; i < _children.size(); ++i)
    _children[i]->PrintTree(log);
  if (_children.size() > 0)
  {
    if (_parent && !madeSpace)
    {
      log("%s", indentation.c_str());
      madeSpace = true;
    }
    if (indentation.length() > newindent.length())
      indentation = indentation.substr(newindent.length());
    else
      indentation = "";
  }
}

void Object::PrintTree() const
{
  PrintTree(Log::Debug);
}

std::string Object::Name() const
{
  return _name;
}

unsigned Object::ChildrenCount() const
{
  return _children.size();
}

std::vector<Object *> &Object::Children()
{
  return _children;
}

bool Object::HasAncestor(const Object *other) const
{
  const Object *p = this;
  while ((p = p->Parent()))
    if (p == other)
      return true;
  return false;
}

void Object::PopulateDebugger()
{
  ImGui::Text("Children: %d", ChildrenCount());
}

void Object::OnStart()
{
}

void Object::OnActivate()
{
}

void Object::OnUpdate()
{
}

void Object::OnEarlyUpdate()
{
  if (!Active())
    return;
  for (unsigned i = 0; i < _children.size(); ++i)
    _children[i]->OnEarlyUpdate();
}

void Object::OnLateUpdate()
{
  if (!Active())
    return;
  for (unsigned i = 0; i < _children.size(); ++i)
    _children[i]->OnLateUpdate();
}

void Object::OnDeactivate()
{
}

void Object::OnEnd()
{
}

void Object::OnCollision(Physics::Collision c)
{
}

void Object::OnMouseEnter()
{
}

void Object::OnMouseExit()
{
}

void Object::OnMouseOver()
{
}

void Object::OnMouseClick()
{
}

void Object::OnMouseRelease()
{
}
} // namespace Object
} // namespace Aspen
