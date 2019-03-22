#ifndef __TRANSFORM_HPP
#define __TRANSFORM_HPP

#include "Object.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Transform namespace
///        To be replaced after running `make newfile NEWFILE=file`
namespace Transform
{
/// \brief Transform class
class Transform : public Object::Object
{
  /// \brief X position
  float _posx;
  /// \brief Y position
  float _posy;
  /// \brief Rotation angle
  double _r;
  /// \brief X scale
  float _scalex;
  /// \brief Y scale
  float _scaley;

public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object creating this Object
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Transform(Object *parent = nullptr, std::string name = "Transform");

  /// \brief Sets the position
  /// \param x New x position
  /// \param y New y position
  void SetPosition(float x, float y);
  /// \brief Sets the x position
  /// \param x New x position
  void SetXPosition(float x);
  /// \brief Sets the y position
  /// \param y New y position
  void SetYPosition(float y);
  /// \brief Sets the rotation
  /// \param r New rotation
  void SetRotation(double r);
  /// \brief Sets the scale
  /// \param x New x scale
  /// \param y New y scale
  void SetScale(float x, float y);
  /// \brief Sets the x scale
  /// \param x New x scale
  void SetXScale(float x);
  /// \brief Sets the y scale
  /// \param y New y scale
  void SetYScale(float y);

  /// \brief Modifies the position
  /// \param x X position modifier
  /// \param y Y position modifier
  void ModifyPosition(float x, float y);
  /// \brief Modifies the x position
  /// \param x X position modifier
  void ModifyXPosition(float x);
  /// \brief Modifies the y position
  /// \param y Y position modifier
  void ModifyYPosition(float y);
  /// \brief Modifies the rotation
  /// \param r Rotation modifier
  void ModifyRotation(double r);
  /// \brief Modifies the scale
  /// \param x X scale modifier
  /// \param y Y scale modifier
  void ModifyScale(float x, float y);
  /// \brief Modifies the x scale
  /// \param x X scale modifier
  void ModifyXScale(float x);
  /// \brief Modifies the y scale
  /// \param y Y scale modifier
  void ModifyYScale(float y);

  /// \brief Gets the total x position in world space
  /// \return World space x position
  float GetXPosition() const;
  /// \brief Gets the total y position in world space
  /// \return World space y position
  float GetYPosition() const;
  /// \brief Gets the total rotation in world space
  /// \return World space rotation
  double GetRotation() const;
  /// \brief Gets the total x scale in world space
  /// \return World space x scale
  float GetXScale() const;
  /// \brief Gets the total y scale in world space
  /// \return World space y scale
  float GetYScale() const;

  /// \brief Gets the total x position in world space while taking the camera into account
  /// \return World space x position
  float GetXPosition(const Transform *camera) const;
  /// \brief Gets the total y position in world space while taking the camera into account
  /// \return World space y position
  float GetYPosition(const Transform *camera) const;
  /// \brief Gets the total rotation in world space while taking the camera into account
  /// \return World space rotation
  double GetRotation(const Transform *camera) const;
  /// \brief Gets the total x scale in world space while taking the camera into account
  /// \return World space x scale
  float GetXScale(const Transform *camera) const;
  /// \brief Gets the total y scale in world space while taking the camera into account
  /// \return World space y scale
  float GetYScale(const Transform *camera) const;

  /// \brief Gets the inverse x position in world space
  /// \return World space x position
  float GetInverseXPosition() const;
  /// \brief Gets the inverse y position in world space
  /// \return World space y position
  float GetInverseYPosition() const;
  /// \brief Gets the inverse rotation in world space
  /// \return World space rotation
  double GetInverseRotation() const;
  /// \brief Gets the inverse x scale in world space
  /// \return World space x scale
  float GetInverseXScale() const;
  /// \brief Gets the inverse y scale in world space
  /// \return World space y scale
  float GetInverseYScale() const;

  /// \brief Gets the local x position
  /// \return Local x position
  float GetLocalXPosition() const;
  /// \brief Gets the local y position
  /// \return Local y position
  float GetLocalYPosition() const;
  /// \brief Gets the local rotation
  /// \return Local rotation
  double GetLocalRotation() const;
  /// \brief Gets the local x scale
  /// \return Local x scale
  float GetLocalXScale() const;
  /// \brief Gets the local y scale
  /// \return Local y scale
  float GetLocalYScale() const;

  /// \brief Gets the inverse local x position
  /// \return Local x position
  float GetLocalInverseXPosition() const;
  /// \brief Gets the inverse local y position
  /// \return Local y position
  float GetLocalInverseYPosition() const;
  /// \brief Gets the inverse local rotation
  /// \return Local rotation
  double GetLocalInverseRotation() const;
  /// \brief Gets the inverse local x scale
  /// \return Local x scale
  float GetLocalInverseXScale() const;
  /// \brief Gets the inverse local y scale
  /// \return Local y scale
  float GetLocalInverseYScale() const;

  /// \brief Returns the inverse of this Transform
  /// \return Inverse of this Transform
  Transform Inverse() const;

  /// \brief Combines two Transform objects
  /// \param rhs Transform to the right of the operator
  /// \return Result of the combination
  Transform operator+(const Transform &rhs) const;
  /// \brief Combines two Transform objects and stores the result in this Transform
  /// \param rhs Transform to the right of the operator
  /// \return Result of the combination
  Transform &operator+=(const Transform &rhs);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};
} // namespace Transform
} // namespace Aspen

#endif
