#ifndef __GRAPHICS_HPP
#define __GRAPHICS_HPP
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "Log.hpp"
#include "Object.hpp"
#include <map>

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

  Uint8 Red();
  Uint8 Green();
  Uint8 Blue();
  Uint8 Alpha();

  void Red(Uint8 r);
  void Green(Uint8 g);
  void Blue(Uint8 b);
  void Alpha(Uint8 a);

  operator SDL_Color();
};

namespace Colors
{
const Color BLACK = Color(0x000000FF);
const Color RED   = Color(0xFF0000FF);
const Color GREEN = Color(0x00FF00FF);
const Color BLUE  = Color(0x0000FFFF);
const Color WHITE = Color(0xFFFFFFFF);
}

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

class UniformSpritesheet : public Sprite
{
  SDL_Rect _frame;
  int _framecount;

public:
  UniformSpritesheet(Object *parent = nullptr, std::string name = "UniformSpritesheet");
  /// \brief Constructor
  /// \param path Path of file to load
  /// \param parent Parent Object to be passed to Object constructor
  UniformSpritesheet(std::string path, unsigned frameCount, Object *parent = nullptr, std::string name = "UniformSpritesheet");
  UniformSpritesheet(std::string path, unsigned frameWidth, unsigned frameHeight, Object *parent = nullptr, std::string name = "UniformSpritesheet");
  UniformSpritesheet(std::string path, unsigned frameWidth, unsigned frameHeight, unsigned frameCount, Object *parent = nullptr, std::string name = "UniformSpritesheet");
  /// \brief Destructor
  ~UniformSpritesheet();

  void operator()();

  int GetFrameCount();
  SDL_Rect GetClipRectangle(int frame);

  void PopulateDebugger();
};

class Animation : public Object::Object
{
  int _currentFrame;
  float _delay;
  float _remainingDelay;

public:
  Animation(Object *parent = nullptr, std::string name = "Animation");
  /// \brief Constructor
  /// \param path Path of file to load
  /// \param parent Parent Object to be passed to Object constructor
  Animation(UniformSpritesheet *spritesheet, float frameDelay = 0.05f, Object *parent = nullptr, std::string name = "Animation");
  /// \brief Destructor
  ~Animation();

  void operator()();

  int GetFrameCount();

  void PopulateDebugger();
};

class FontCache : public Object::Object
{
  std::map<std::string, std::string> _paths;
  std::map<std::pair<std::string, int>, TTF_Font *> _fonts;

public:
  FontCache(Object *parent = nullptr, std::string name = "FontCache");
  ~FontCache();

  void LoadFont(std::string path);
  void LoadFont(std::string path, std::string name);
  void UnloadFont(std::string name);
  TTF_Font *GetFont(std::string name, int size);
};

class Text : public Object::Object
{
  std::string _text;
  std::string _font;
  SDL_Texture *_tex;
  Color _c;
  SDL_Rect _rect;
  int _size;

public:
  Text(Object *parent = nullptr, std::string name = "Text");
  Text(std::string text, Object *parent = nullptr, std::string name = "Text");
  Text(std::string text, std::string font, int size, Object *parent = nullptr, std::string name = "Text");
  Text(std::string text, std::string font, int size, Color c, Object *parent = nullptr, std::string name = "Text");
  ~Text();

  void operator()();

  std::string GetText();
  void SetText(std::string text);

  std::string GetFont();
  void SetFont(std::string font);

  Color GetColor();
  void SetColor(Color c);

  SDL_Rect GetRect();

  int GetSize();
  void SetSize(int size);

  void GenerateTexture();
  SDL_Texture *GetTexture();

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

  void OnEarlyUpdate();
  void OnLateUpdate();

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
  void DrawSprite(Sprite *sprite, SDL_Rect clip);

  void DrawText(Text *text);
  void DrawText(Text *text, SDL_Rect clip);

  /// \brief Frees the Window and shuts down SDL
  void End();

  void PopulateDebugger();
};
} // namespace Graphics
} // namespace Aspen

#endif
