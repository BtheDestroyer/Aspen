#define __OBJECT_CPP

#include "Object.hpp"
#include "Log.hpp"
#include <algorithm>
#include <iomanip>

#undef __OBJECT_CPP

namespace Aspen
{
namespace Object
{
int Object::_count = 0;

Object::Object(Object *parent)
    : _name("Object"), _parent(parent)
{
  ++_count;
  std::stringstream str;
  str << std::setw(32) << std::setfill(' ') << std::left << "Creating " << _name << ": " << this << "  ";
  str << _count;
  Log::Debug(str);
  _valid = true;
}

Object::~Object()
{
  --_count;
  std::stringstream str;
  str << std::setw(32) << std::setfill(' ') << std::left << "Destroying " << _name << ": " << this << "  ";
  str << _count;
  Log::Debug(str);
  if (_count == 0)
    Log::Debug("All clean :D");
  End();
  for (Object *child : _children)
    delete child;
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

void Object::operator()()
{
  for (Object *child : _children)
  {
    (*child)();
    if (!child->Valid())
    {
      RemoveChild(child);
      delete child;
    }
  }
}

void Object::AddChild(Object *child)
{
  if (std::find(_children.begin(), _children.end(), child) == _children.end())
  {
    child->SetParent(this);
    _children.push_back(child);
  }
}

void Object::RemoveChild(Object *child)
{
  if (!child)
    return;
  std::vector<Object *>::iterator it = std::find(_children.begin(), _children.end(), child);
  if (it != _children.end())
  {
    (*it)->_parent = nullptr;
    _children.erase(it);
  }
}

void Object::RemoveChild(int index)
{
  if (index < _children.size())
  {
    _children[index]->_parent = nullptr;
    _children.erase(_children.begin() + index);
  }
}

Object *Object::operator[](int index)
{
  if (index < _children.size())
    return _children[index];
  return nullptr;
}

const bool &Object::Valid() const
{
  return _valid;
}
Object::operator bool() const
{
  return Valid();
}

void Object::End()
{
  if (!Valid())
    return;

  for (Object *child : _children)
    child->End();
  _valid = false;
}

void Object::PrintTree(Log::Log log)
{
  static std::string indentation = "";
  log("%s\\... %s (%p)", indentation.c_str(), _name.c_str(), this);
  indentation = "| " + indentation;
  for (Object *child : _children)
  {
    child->PrintTree(log);
  }
  indentation = indentation.substr(3);
}

} // namespace Object
} // namespace Aspen
