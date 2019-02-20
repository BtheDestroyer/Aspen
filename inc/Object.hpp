#ifndef __OBJECT_HPP
#define __OBJECT_HPP
#include <vector>

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Stub namespace
///        To be replaced after running `make newfile NEWFILE=file`
namespace Object
{
class Object
{
protected:
  static int _count;
  Object *_parent;
  std::vector<Object *> _children;
  bool _valid = false;
  void SetParent(Object *parent);

public:
  Object(Object *parent = nullptr);
  virtual ~Object();

  Object *Parent();
  Object *Root();

  virtual void operator()();

  void AddChild(Object *child);
  template <typename T>
  T *CreateChild()
  {
    T *o = new T;
    AddChild(o);
    return o;
  }
  void RemoveChild(Object *child);
  void RemoveChild(int index);
  Object *operator[](int index);
  template <typename T>
  T *FindChildOfType()
  {
    for (Object *child : _children)
      if (child && dynamic_cast<T *>(child))
        return dynamic_cast<T *>(child);
    return nullptr;
  }
  template <typename T>
  std::vector<T *> FindChildrenOfType()
  {
    std::vector<T *> vec;
    for (Object *child : _children)
      if (child && dynamic_cast<T *>(child))
        vec.push_back(dynamic_cast<T *>(child));
    return vec;
  }

  const bool &Valid() const;
  operator bool() const;
  void End();
};
} // namespace Object
} // namespace Aspen

#endif
