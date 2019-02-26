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

enum COLOR_MASK
{
  RED = 0xFF000000,
  GREEN = 0x00FF0000,
  BLUE = 0x0000FF00,
  ALPHA = 0x000000FF
};

class Color
{
protected:
  unsigned _c;

public:
  Color(int color = 0xFFFFFFFF);
  Color(int r, int g, int b, int a = 0xFF);

  int Red();
  int Green();
  int Blue();
  int Alpha();

  void Red(int r);
  void Green(int g);
  void Blue(int b);
  void Alpha(int a);
};

class Geometry : public Object::Object
{
protected:
  Color _c;
  bool _fill;

public:
  Geometry(Object *parent = nullptr, std::string name = "Geometry");
  Geometry(Color c, bool fill = false, Object *parent = nullptr, std::string name = "Graphics");
  ~Geometry();

  Color &Color();
  void Color(Aspen::Graphics::Color c);
  void Color(int r, int g, int b, int a);
  bool Fill();

  void PopulateDebugger();
};

class Rectangle : public Geometry
{
  SDL_Rect _rect;

public:
  Rectangle(Object *parent = nullptr, std::string name = "Rectangle");
  Rectangle(SDL_Rect rect, Aspen::Graphics::Color c = Aspen::Graphics::Color(), bool fill = false, Object *parent = nullptr, std::string name = "Rectangle");
  ~Rectangle();

  void operator()();

  SDL_Rect &GetRect();

  void PopulateDebugger();
};

class Point : public Geometry
{
  SDL_Point _point;

public:
  Point(Object *parent = nullptr, std::string name = "Point");
  Point(SDL_Point point, Aspen::Graphics::Color c = Aspen::Graphics::Color(), Object *parent = nullptr, std::string name = "Point");
  ~Point();

  void operator()();

  SDL_Point &GetPoint();

  void PopulateDebugger();
};

class Line : public Geometry
{
  SDL_Point _start;
  SDL_Point _end;
  float _center;

public:
  Line(Object *parent = nullptr, std::string name = "Line");
  Line(SDL_Point start, SDL_Point end, float center = 0.5f, Aspen::Graphics::Color c = Aspen::Graphics::Color(), Object *parent = nullptr, std::string name = "Line");
  ~Line();

  void operator()();

  SDL_Point &GetStart();
  SDL_Point &GetEnd();
  float &GetCenter();
  void SetCenter(float center);

  void PopulateDebugger();
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
  SDL_Rect _rect;

public:
  Sprite(Object *parent = nullptr, std::string name = "Sprite");
  /// \brief Constructor
  /// \param path Path of file to load
  /// \param parent Parent Object to be passed to Object constructor
  Sprite(std::string path, Object *parent = nullptr, std::string name = "Sprite");
  /// \brief Destructor
  ~Sprite();

  void End();

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

  SDL_Rect &GetRect();

  void PopulateDebugger();
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
  Color _background;

public:
  /// \brief Constructor
  ///        Calls Graphics::Graphics(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT)
  Graphics(Object *parent = nullptr, std::string name = "Graphics");
  /// \brief Constructor
  ///        Sets up SDL
  ///        Creates a window with width w and height h
  Graphics(int w, int h, Object *parent = nullptr, std::string name = "Graphics");
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
  void DrawRectangle(SDL_Rect *rect, Color c, bool fill = true);

  void DrawPoint(Point *point);
  void DrawPoint(SDL_Point *point, Color c);

  void DrawLine(Line *line);
  void DrawLine(SDL_Point *start, SDL_Point *end, Color c);
  /// \brief Draws a given Sprite at 0, 0
  /// \param sprite Sprite to draw
  void DrawSprite(Sprite *sprite);

  /// \brief Frees the Window and shuts down SDL
  void End();

  void PopulateDebugger();
};
} // namespace Graphics
} // namespace Aspen

#endif
