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
class Transform : public Object::Object
{
  float _posx;
  float _posy;
  float _r;
  float _scalex;
  float _scaley;
public:
  Transform(Object *parent = nullptr, std::string name = "Transform");

  void SetPosition(float x, float y);
  void SetXPosition(float x);
  void SetYPosition(float y);
  void SetRotation(float r);
  void SetScale(float x, float y);
  void SetXScale(float x);
  void SetYScale(float y);

  void ModifyPosition(float x, float y);
  void ModifyXPosition(float x);
  void ModifyYPosition(float y);
  void ModifyRotation(float r);
  void ModifyScale(float x, float y);
  void ModifyXScale(float x);
  void ModifyYScale(float y);

  float GetXPosition() const;
  float GetYPosition() const;
  float GetRotation() const;
  float GetXScale() const;
  float GetYScale() const;

  Transform operator+(const Transform &rhs);
  Transform &operator+=(const Transform &rhs);
};
} // namespace Transform
} // namespace Aspen

#endif
