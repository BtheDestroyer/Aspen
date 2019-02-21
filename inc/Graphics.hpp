#ifndef __GRAPHICS_HPP
#define __GRAPHICS_HPP
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Log.hpp"
#include "Object.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief Graphics namespace
///        Controls and facilitates graphics processing
namespace Graphics
{
/// \brief Default created window width
extern const int DEFAULT_WINDOW_WIDTH;
/// \brief Default created window height
extern const int DEFAULT_WINDOW_HEIGHT;

class Geometry : public Object::Object
{
protected:
  int _r;
  int _g;
  int _b;
  int _a;
  bool _fill;

public:
  Geometry(std::string name = "Geometry", Object *parent = nullptr);
  Geometry(std::string name, Object *parent, int r, int g, int b, int a = 0xFF, bool fill = false);
  ~Geometry();

  void SetColor(int r, int g, int b, int a);
  int Red();
  int Green();
  int Blue();
  int Alpha();
  bool Fill();
};

class Rectangle : public Geometry
{
  SDL_Rect _rect;

public:
  Rectangle(Object *parent = nullptr, SDL_Rect rect = {0, 0, 1, 1});
  ~Rectangle();

  void operator()();

  SDL_Rect &Rect();
};

/// \brief Sprite class
class Sprite : public Object::Object
{
  /// \brief Path of file loaded as a sprite
  std::string _path;
  /// \brief Surface loaded from _path
  SDL_Surface *_surface;
  /// \brief Texture generated from _surface
  SDL_Texture *_tex;

public:
  /// \brief Constructor
  /// \param path Path of file to load
  /// \param parent Parent Object to be passed to Object constructor
  Sprite(std::string path, Object *parent = nullptr);
  /// \brief Destructor
  ~Sprite();

  /// \brief Draws the sprite to the parent Object's window if parent is of type Graphics
  void operator()();

  /// \brief Wrapper for Object::SetParent
  /// \param parent New parent
  void SetParent(Object *parent);

  void GenerateTexture();

  /// \brief Gets the path passed into the constructor
  /// \return Const reference to _path
  const std::string &GetPath() const;
  /// \brief Gets the loaded surface
  /// \return _surface
  SDL_Surface *GetSurface();
  /// \brief Gets the generated texture
  /// \return _tex
  SDL_Texture *GetTexture();
};

/// \brief Graphics class
///        Holds other classes from Aspen::Graphics as children to draw them to the screen
class Graphics : public Object::Object
{
  /// \brief Number of Graphics Objects created
  ///        Used for determining if SDL should be initialized or closed
  static unsigned _gcount;
  /// \brief Window to be displayed
  SDL_Window *_window = nullptr;
  /// \brief Surface of _window
  SDL_Surface *_surface = nullptr;
  SDL_Renderer *_renderer = nullptr;

public:
  /// \brief Constructor
  ///        Calls Graphics::Graphics(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
  Graphics();
  /// \brief Constructor
  ///        Sets up SDL
  ///        Creates a window with width w and height h
  Graphics(int w, int h);
  /// \brief Destructor
  ~Graphics();

  /// \brief Clears the window, updates all children, then swaps the framebuffers
  void operator()();

  /// \brief Sets the window's background color
  /// \param r Red value from 0x00 to 0xFF
  /// \param g Green value from 0x00 to 0xFF
  /// \param b Blue value from 0x00 to 0xFF
  void SetBGColor(int r, int g, int b);

  /// \brief Gets the surface of window
  /// \return _surface
  SDL_Surface *GetSurface();
  /// \brief Gets the window
  /// \return _window
  SDL_Window *GetWindow();
  SDL_Renderer *GetRenderer();

  void DrawRectangle(Rectangle *rect);
  void DrawRectangle(SDL_Rect *rect, int r, int g, int b, int a, bool fill = true);
  /// \brief Draws a given Sprite at 0, 0
  /// \param sprite Sprite to draw
  void DrawSprite(Sprite *sprite);

  /// \brief Frees the Window and shuts down SDL
  void End();
};
} // namespace Graphics
} // namespace Aspen

#endif
