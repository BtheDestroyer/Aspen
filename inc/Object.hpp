#ifndef __OBJECT_HPP
#define __OBJECT_HPP
#include <vector>
#include "Log.hpp"
#include "Collision.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Forward declaration
namespace Transform
{
/// \brief Forward declaration
class Transform;
}; // namespace Transform
/// \brief Forward declaration
namespace Physics
{
/// \brief Forward declaration
class Collider;
/// \brief Forward declaration
class Rigidbody;
}; // namespace Physics

/////////////////////////////////////////////////////////

/// \brief Object namespace
///        Contains the Object base class for most classes of Aspen
namespace Object
{
/// \brief General base class
///        Allows for parent/child relationship trees
class Object
{
protected:
  /// \brief Name of object
  const std::string _name;
  /// \brief Total number of Objects in existence
  static int _count;
  /// \brief Parent/owner of this Object
  Object *_parent;
  /// \brief List of children Objects
  std::vector<Object *> _children;
  /// \brief Determines if the Object is valid
  ///        Derived classes can set this to false in their Constructors if they couldn't be created properly
  bool _valid;
  /// \brief Determines if the Object is currently updated
  bool _active;
  /// \brief Determines if the Object has been started
  ///        Set to true during the first update
  bool _started;

  /// \brief First child Transform::Transform
  Transform::Transform *_transform;
  /// \brief First child Physics::Collider
  Physics::Collider *_collider;
  /// \brief First child Physics::Rigidbody
  Physics::Rigidbody *_rigidbody;

  /// \brief Sets _parent to the given Object
  ///        Used by AddChild, CreateChild, etc.
  void SetParent(Object *parent);
  /// \brief Runs OnStart if the object is currently active for the first time
  void TriggerOnStart();
  /// \brief Runs OnActivate if the object is currently active
  void TriggerOnActivate();
  /// \brief Runs OnDeactivate if the object is currently active (being deactivated)
  void TriggerOnDeactivate();

public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object creating this Object
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Object(Object *parent = nullptr, std::string name = "Object");
  /// \brief Destructor
  ///        This will End and then delete all child Objects
  virtual ~Object();

  /// \brief Gets the parent of this Object
  /// \return _parent
  const Object *Parent() const;
  /// \brief Gets the parent of this Object
  /// \return _parent
  Object *Parent();
  /// \brief Gets the root Object of the parent/child tree this Object is a part of
  /// \return Root Object of this Object's tree
  const Object *Root() const;
  /// \brief Gets the root Object of the parent/child tree this Object is a part of
  /// \return Root Object of this Object's tree
  Object *Root();

  /// \brief Gets the first child Transform::Transform
  /// \return Child Transform
  Transform::Transform *GetTransform();
  /// \brief Gets the first child Transform::Transform
  /// \return Child Transform
  const Transform::Transform *GetTransform() const;
  /// \brief Gets the first child Physics::Collider
  /// \return Child Collider
  Physics::Collider *GetCollider();
  /// \brief Gets the first child Physics::Collider
  /// \return Child Collider
  const Physics::Collider *GetCollider() const;
  /// \brief Gets the first child Physics::Rigidbody
  /// \return Child Rigidbody
  Physics::Rigidbody *GetRigidbody();
  /// \brief Gets the first child Physics::Rigidbody
  /// \return Child Rigidbody
  const Physics::Rigidbody *GetRigidbody() const;

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
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
    T *o = new T(this);
    AddChild(o);
    return o;
  }
  /// \brief Creates a new child of type T
  ///        Useful for creating an object with no constructor parameters and/or modifying it later
  /// \tparam T Type of child to create
  ///           Must inherit Object
  /// \param name Name of the new object
  /// \return Newly created Object
  template <typename T>
  T *CreateChild(std::string name)
  {
    T *o = new T(this, name);
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
  void RemoveChild(unsigned index);
  /// \brief Gets the child at index from this Object's list of children
  /// \param index Index of child Object to get
  /// \return Child object at index from this Object's list of children
  ///         This will return nullptr if index is out of range of the Object's list of children
  Object *operator[](unsigned index);
  /// \brief Gets the index of a given child in this Object's list of children
  /// \param child Child Object to get the index of
  /// \return Index of child
  ///         This will return -1 if child is not in the Object's list of children
  int operator[](Object *child);
  /// \brief Gets the youngest child from this Object's list of children
  /// \return Last child of Object
  ///         This will return nullptr if the Object has no children
  Object *GetLastChild();
  /// \brief Determines the generation of this object in the parent/child tree
  /// \return Generation index of this Object (0 = root)
  int ParentCount();

  /// \brief Finds the closest ancestor Object of a type applicable to that which was requested
  /// \tparam T Type of child to find
  ///           Must inherit Object
  /// \return The closest ancestor of type T
  ///         If no ancestors match the given type, nullptr is returned
  ///         If `class A : public Object` and `class B : public A`, then `FindObjectOfType<A>()` will return whichever was added earliest - `A*` or `B*` - as an `A*`
  template <typename T>
  T *FindAncestorOfType() const
  {
    Object *p = _parent;
    while (p)
    {
      if (dynamic_cast<T *>(p))
        return dynamic_cast<T *>(p);
      p = p->Parent();
    }
    return nullptr;
  }
  /// \brief Finds the first child Object of a type applicable to that which was requested
  /// \tparam T Type of child to find
  ///           Must inherit Object
  /// \return The first child of type T
  ///         If no children match the given type, nullptr is returned
  ///         If `class A : public Object` and `class B : public A`, then `FindObjectOfType<A>()` will return whichever was added earliest - `A*` or `B*` - as an `A*`
  template <typename T>
  const T *FindChildOfType() const
  {
    for (unsigned i = 0; i < _children.size(); ++i)
      if (dynamic_cast<T *>(_children[i]))
        return dynamic_cast<T *>(_children[i]);
    return nullptr;
  }
  /// \brief Finds the first child Object of a type applicable to that which was requested
  /// \tparam T Type of child to find
  ///           Must inherit Object
  /// \return The first child of type T
  ///         If no children match the given type, nullptr is returned
  ///         If `class A : public Object` and `class B : public A`, then `FindObjectOfType<A>()` will return whichever was added earliest - `A*` or `B*` - as an `A*`
  template <typename T>
  T *FindChildOfType()
  {
    for (unsigned i = 0; i < _children.size(); ++i)
      if (dynamic_cast<T *>(_children[i]))
        return dynamic_cast<T *>(_children[i]);
    return nullptr;
  }

  /// \brief Finds all children Objects of a type applicable to that which was requested
  /// \tparam T Type of children to find
  ///           Must inherit Object
  /// \return All children of type T
  ///         If no children match the given type, nullptr is returned
  ///         If `class A : public Object` and `class B : public A`, then `FindObjectOfType<A>()` will return both `A*` and `B*` as `A*`
  template <typename T>
  const std::vector<T *> FindChildrenOfType() const
  {
    std::vector<T *> vec;
    for (unsigned i = 0; i < _children.size(); ++i)
      if (dynamic_cast<T *>(_children[i]))
        vec.push_back(dynamic_cast<T *>(_children[i]));
    return vec;
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
    for (unsigned i = 0; i < _children.size(); ++i)
      if (dynamic_cast<T *>(_children[i]))
        vec.push_back(dynamic_cast<T *>(_children[i]));
    return vec;
  }
  /// \brief Recursively finds all descendent Objects of a type applicable to that which was requested
  /// \tparam T Type of descendent to find
  ///           Must inherit Object
  /// \return All children of type T
  ///         If no descendents match the given type, an empty vector is returned
  ///         If `class A : public Object` and `class B : public A`, then `FindObjectOfType<A>()` will return both `A*` and `B*` as `A*`
  template <typename T>
  const std::vector<T *> FindDescendentsOfType() const
  {
    std::vector<T *> vec;
    for (unsigned i = 0; i < _children.size(); ++i)
    {
      if (dynamic_cast<T *>(_children[i]))
        vec.push_back(dynamic_cast<T *>(_children[i]));
      std::vector<T *> cvec = _children[i]->FindDescendentsOfType<T>();
      vec.insert(vec.end(), cvec.begin(), cvec.end());
    }
    return vec;
  }
  /// \brief Recursively finds all descendent Objects of a type applicable to that which was requested
  /// \tparam T Type of descendent to find
  ///           Must inherit Object
  /// \return All children of type T
  ///         If no descendents match the given type, an empty vector is returned
  ///         If `class A : public Object` and `class B : public A`, then `FindObjectOfType<A>()` will return both `A*` and `B*` as `A*`
  template <typename T>
  std::vector<T *> FindDescendentsOfType()
  {
    std::vector<T *> vec;
    for (unsigned i = 0; i < _children.size(); ++i)
    {
      if (dynamic_cast<T *>(_children[i]))
        vec.push_back(dynamic_cast<T *>(_children[i]));
      std::vector<T *> cvec = _children[i]->FindDescendentsOfType<T>();
      vec.insert(vec.end(), cvec.begin(), cvec.end());
    }
    return vec;
  }

  /// \brief Determines if the Object is valid
  /// \return Const reference to _valid
  const bool &Valid() const;
  /// \brief Determines if the Object is active
  /// \return True if both valid and active
  ///         False otherwise
  bool Active() const;
  /// \brief Sets active status of the object
  ///        Triggers OnActivate or OnDeactivate appropriately
  /// \param active New active status
  void SetActive(bool active);
  /// \brief Activates the object
  ///        Triggers OnActivate appropriately
  void Activate();
  /// \brief Deactivates the object
  ///        Triggers OnDeactivate appropriately
  void Deactivate();

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
  virtual void End();

  /// \brief Uses Log::Log to print the parent/children tree of this Object
  /// \param log Log::Log to use
  void PrintTree(Log::Log &log) const;
  /// \brief Uses Log::Log to print the parent/children tree of this Object
  ///        Uses Log::Debug
  void PrintTree() const;

  /// \brief Gets the Object's name
  /// \return Object name
  std::string Name() const;

  /// \brief Determines the number of immediate children the Object has
  /// \return Number of children owned by the Object
  unsigned ChildrenCount() const;
  /// \brief Gets the list of Objects that are children of this Object
  /// \return Children owned by the Object
  std::vector<Object *> &Children();

  /// \brief Determines if the provided Object is an ancestor of this Object
  /// \param other Potential ancestor of this Object
  /// \return True if other is an ancestor of this Object
  ///         False otherwise
  bool HasAncestor(const Object *other) const;

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  virtual void PopulateDebugger();

  /// \brief Run when the Object is created
  virtual void OnStart();
  /// \brief Run when the Object is activated
  ///        Run after OnStart
  virtual void OnActivate();
  /// \brief Run every frame
  virtual void OnUpdate();
  /// \brief Run before OnUpdate
  virtual void OnEarlyUpdate();
  /// \brief Run after OnUpdate
  virtual void OnLateUpdate();
  /// \brief Run when the Object is deactivated
  ///        Run before OnEnd
  virtual void OnDeactivate();
  /// \brief Run when the Object is ended/destroyed
  virtual void OnEnd();

  /// \brief Run when a collision occurs
  /// \param c Collision that occured
  virtual void OnCollision(Physics::Collision c);
  /// \brief Run when the mouse enters the Object
  virtual void OnMouseEnter();
  /// \brief Run while the mouse is over the Object
  virtual void OnMouseOver();
  /// \brief Run when the mouse leaves the Object
  virtual void OnMouseExit();
  /// \brief Run when the mouse is clicked while over the Object
  virtual void OnMouseClick();
  /// \brief Run when the mouse is released while over the Object
  virtual void OnMouseRelease();
};
} // namespace Object
} // namespace Aspen

#endif
