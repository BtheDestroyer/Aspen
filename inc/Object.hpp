#ifndef __OBJECT_HPP
#define __OBJECT_HPP
#include <vector>

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Object namespace
///        Contains the Object base class for most classes of Aspen
namespace Object
{
/// \brief General base class
///        Allows for parent/child relationship trees
class Object
{
protected:
  /// \brief Total number of Objects in existence
  static int _count;
  /// \brief Parent/owner of this Object
  Object *_parent;
  /// \brief List of children Objects
  std::vector<Object *> _children;
  /// \brief Determines if the Object is valid
  ///        Derived classes can set this to false in their Constructors if they couldn't be created properly
  bool _valid = false;
  /// \brief Sets _parent to the given Object
  ///        Used by AddChild, CreateChild, etc.
  void SetParent(Object *parent);

public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object creating this Object
  Object(Object *parent = nullptr);
  /// \brief Destructor
  ///        This will End and then delete all child Objects
  virtual ~Object();

  /// \brief Gets the parent of this Object
  /// \return _parent
  Object *Parent();
  /// \brief Gets the root Object of the parent/child tree this Object is a part of
  /// \return Root Object of this Object's tree
  Object *Root();

  /// \brief Updates this object and all of its children
  ///        Derived classes should call this at some point in their operator()
  ///        This won't run if _valid is false
  virtual void operator()();

  /// \brief Adds child to this Object's list of children
  ///        This will also set child's _parent to this
  ///        This will do nothing if child is already in the list of children
  ///        Useful for adding an already created Object as a child or passing in the return operator new with parameters
  /// \param child Object to add to list of children
  void AddChild(Object *child);
  /// \brief Creates a new child of type T
  ///        Useful for creating an object with no constructor parameters and/or modifying it later
  /// \tparam T Type of child to create
  ///           Must inherit Object
  /// \return Newly created Object
  template <typename T>
  T *CreateChild()
  {
    T *o = new T;
    AddChild(o);
    return o;
  }
  /// \brief Removes child from this Object's list of children
  ///        This will also set child's _parent to nullptr
  ///        This will do nothing if child is not in the Object's list of children
  /// \param child Object to remove from list of children
  void RemoveChild(Object *child);
  /// \brief Removes the child at index from this Object's list of children
  ///        This will also set the child's _parent to nullptr
  ///        This will do nothing if index is out of range of the Object's list of children
  /// \param index Index of Object to remove from list of children
  void RemoveChild(int index);
  /// \brief Gets the child at index from this Object's list of children
  /// \param index Index of child Object to get
  /// \return Child object at index from this Object's list of children
  ///        This will return nullptr if index is out of range of the Object's list of children
  Object *operator[](int index);
  /// \brief Finds the first child Object of a type applicable to that which was requested
  /// \tparam T Type of child to find
  ///           Must inherit Object
  /// \return The first child of type T
  ///         If no children match the given type, nullptr is returned
  ///         If `class A : public Object` and `class B : public A`, then `FindObjectOfType<A>()` will return whichever was added earliest - `A*` or `B*` - as an `A*`
  template <typename T>
  T *FindChildOfType()
  {
    for (Object *child : _children)
      if (child && dynamic_cast<T *>(child))
        return dynamic_cast<T *>(child);
    return nullptr;
  }
  /// \brief Finds all children Objects of a type applicable to that which was requested
  /// \tparam T Type of children to find
  ///           Must inherit Object
  /// \return All children of type T
  ///         If no children match the given type, nullptr is returned
  ///         If `class A : public Object` and `class B : public A`, then `FindObjectOfType<A>()` will return both `A*` and `B*` as `A*`
  template <typename T>
  std::vector<T *> FindChildrenOfType()
  {
    std::vector<T *> vec;
    for (Object *child : _children)
      if (child && dynamic_cast<T *>(child))
        vec.push_back(dynamic_cast<T *>(child));
    return vec;
  }

  /// \brief Determines if the Object is valid
  /// \return Const reference to _valid
  const bool &Valid() const;
  /// \brief Converts Object to bool by calling Valid
  ///        Example:
  ///        ```
  ///        Object o;
  ///        while (o)
  ///          o();
  ///        ```
  operator bool() const;
  /// \brief Shuts down and invalidates Object and all of its children
  ///        An invalid child Object will be deleted by their parent after they update
  void End();
};
} // namespace Object
} // namespace Aspen

#endif
