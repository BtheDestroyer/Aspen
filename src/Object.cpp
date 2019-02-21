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

Object::Object(Object *parent, std::string name)
    : _name(name), _parent(parent)
{
  ++_count;
  std::stringstream str;
  str << "Creating " << _name << ": " << this << "  ";
  str << _count;
  Log::Debug(str);
  _valid = true;
}

Object::~Object()
{
  --_count;
  std::stringstream str;
  str << "Destroying " << _name << ": " << this << "  ";
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

void Object::PrintTree(Log::Log &log)
{
  static std::string indentation = "";
  static const std::string newindent = "  |    ";
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
        indentation = "       " + indentation;
    }
    else
    {
      log("%s+--- %s (%p) (%s)", indentation.c_str(), _name.c_str(), this, Valid() ? "Valid" : "Ended");
      if (_children.size() > 0)
        indentation = newindent + indentation;
    }
  }
  for (unsigned i = 0; i < _children.size(); ++i)
    _children[i]->PrintTree(log);
  if (_children.size() > 0)
  {
    if (_parent)
      log("%s", indentation.c_str());
    if (indentation.length() > newindent.length())
      indentation = indentation.substr(newindent.length());
    else
      indentation = "";
  }
}

void Object::PrintTree()
{
  PrintTree(Log::Debug);
}
} // namespace Object
} // namespace Aspen