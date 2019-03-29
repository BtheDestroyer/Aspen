#ifndef __UI_HPP
#define __UI_HPP
#include "Object.hpp"
#include "Graphics.hpp"
#include "Transform.hpp"
#include "Physics.hpp"
#include <functional>

#include "Input.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Graphics namespace
///        Controls and facilitates graphics processing
namespace Graphics
{
/// \brief UI namespace
namespace UI
{
/// \brief Text class
class Text : public Object::Object
{
  /// \brief Text to draw
  std::string _text;
  /// \brief Font name in which to draw text
  std::string _font;
  /// \brief Texture text is drawn to
  SDL_Texture *_tex;
  /// \brief Color of text
  Color _c;
  /// \brief Rectangle to draw _tex with
  SDL_Rect _rect;
  /// \brief Size of text
  int _size;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Text(Object *parent = nullptr, std::string name = "Text");
  /// \brief Constructor
  /// \param text Text to draw
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Text(std::string text, Object *parent = nullptr, std::string name = "Text");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param text Text to draw
  /// \param font Font in which to draw
  /// \param size Size of the text in pt
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Text(std::string text, std::string font, int size, Object *parent = nullptr, std::string name = "Text");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param text Text to draw
  /// \param font Font in which to draw
  /// \param size Size of the text in pt
  /// \param c Color of text
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Text(std::string text, std::string font, int size, Color c, Object *parent = nullptr, std::string name = "Text");
  /// \brief Destructor
  ~Text();

  /// \brief Draws the sprite to the parent Object's window if parent is of type Graphics
  void operator()();

  /// \brief Gets the text of the object
  /// \return Text of the object
  std::string GetText();
  /// \brief Sets the text of the object
  /// \param text New text of the object
  void SetText(std::string text);

  /// \brief Gets the font of the text
  /// \return Font of the text
  std::string GetFont();
  /// \brief Sets the font of the text
  /// \param font New font of the text
  void SetFont(std::string font);

  /// \brief Gets the color of the text
  /// \return Color of the text
  Color GetColor();
  /// \brief Sets the color of the text
  /// \param c New color of the text
  void SetColor(Color c);

  /// \brief Gets the rectangle _tex is drawn to
  /// \return Reference to _rect
  SDL_Rect GetRect();

  /// \brief Gets the text size
  /// \return Size of the text
  int GetSize();
  /// \brief Sets the text size
  /// \param size New size of the text
  void SetSize(int size);

  /// \brief Generates _tex
  void GenerateTexture();
  /// \brief Gets the generated texture
  /// \return Generated texture
  SDL_Texture *GetTexture();

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief Forward declaration
class Button;
/// \breif Synonym/shorthand function typedef
///        Used by buttons when they're clicked
typedef std::function<void(Button *)> ButtonCallback;

class Button : public Object::Object
{
  std::string _text;
  Text *_textObj;
  Physics::AABBCollider *_collider;
  Rectangle *_rectangle;
  ButtonCallback _onClick;

public:
  Button(Object *parent = nullptr, std::string name = "Button");
  Button(std::string text, Object *parent = nullptr, std::string name = "Button");
  Button(std::string text, int size, Object *parent = nullptr, std::string name = "Button");

  void SetText(std::string text, int size);
  void SetText(std::string text, std::string font);
  void SetText(std::string text, int size, std::string font);
  void SetText(std::string text);

  void SetOnClick(ButtonCallback cb);

  void OnMouseEnter();
  void OnMouseExit();
  void OnMouseClick();
};
} // namespace UI
} // namespace Graphics
} // namespace Aspen

#endif
