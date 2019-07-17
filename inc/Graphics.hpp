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
/// \brief UI namespace
namespace UI
{
/// \brief Forward declaration
class Text;
} // namespace UI

/// \brief Default created window width
extern const int DEFAULT_WINDOW_WIDTH;
/// \brief Default created window height
extern const int DEFAULT_WINDOW_HEIGHT;

/// \brief Masks used to extract individual color components from a color
enum COLOR_MASK
{
  /// \brief Red component of a color
  RED = 0xFF000000,
  /// \brief Green component of a color
  GREEN = 0x00FF0000,
  /// \brief Blue component of a color
  BLUE = 0x0000FF00,
  /// \brief Alpha component of a color
  ALPHA = 0x000000FF
};

/// \brief Color class that is used to store RGBA8 color information
class Color
{
protected:
  /// \brief RGBA8 color
  unsigned _c;

public:
  /// \brief Constructor
  /// \param color RGBA8 color to use as data
  ///              Defaults to fully white
  Color(unsigned color = 0xFFFFFFFF);
  /// \brief Constructor
  /// \param r Red component
  /// \param g Green component
  /// \param b Blue component
  /// \param a Alpha component
  ///          Defaults to fully opaque
  Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0xFF);

  /// \brief Gets red of the color
  /// \return Red component of the color
  Uint8 Red() const;
  /// \brief Gets green of the color
  /// \return Green component of the color
  Uint8 Green() const;
  /// \brief Gets blue of the color
  /// \return Blue component of the color
  Uint8 Blue() const;
  /// \brief Gets alpha of the color
  /// \return Alpha component of the color
  Uint8 Alpha() const;

  /// \brief Sets red of the color
  /// \param r Red component of the color
  void Red(Uint8 r);
  /// \brief Sets green of the color
  /// \param g Green component of the color
  void Green(Uint8 g);
  /// \brief Sets blue of the color
  /// \param b Blue component of the color
  void Blue(Uint8 b);
  /// \brief Sets alpha of the color
  /// \param a Alpha component of the color
  void Alpha(Uint8 a);

  /// \brief Implicitly converts this class to SDL's color class
  operator SDL_Color() const;

  /// \brief Equality operator
  /// \param rhs Color to the right of the operator
  /// \return True if both colors are equivalent
  ///         False otherwise
  bool operator==(const Color &rhs) const;
};

/// \brief Colors namespace
///        Contains some preset colors
namespace Colors
{
/// \brief Black
const Color BLACK = Color(0x000000FF);
/// \brief Red
const Color RED = Color(0xFF0000FF);
/// \brief Green
const Color GREEN = Color(0x00FF00FF);
/// \brief Blue
const Color BLUE = Color(0x0000FFFF);
/// \brief White
const Color WHITE = Color(0xFFFFFFFF);
/// \brief Fully transparent
const Color TRANSPARENT = Color(0x00000000);
} // namespace Colors

/// \brief Geometry class
///        Base class for creating basic geometry like rectangles
class Geometry : public Object::Object
{
protected:
  /// \brief Color of the object
  Color _c;
  /// \brief Determines if the object is filled or outlined
  bool _fill;

public:
  /// \brief Constructor
  /// \param parent Parent Object creating this Object
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Geometry(Object *parent = nullptr, std::string name = "Geometry");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param c Color of the object
  /// \param fill Determines if the object is filled or outlined
  /// \param parent Parent Object creating this Object
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Geometry(Color c, bool fill = false, Object *parent = nullptr, std::string name = "Graphics");
  /// \brief Destructor
  ~Geometry();

  /// \brief Gets the color of the object
  /// \return Reference to _c
  Color &Color();
  /// \brief Sets the color of the object
  /// \param c New color
  void Color(Aspen::Graphics::Color c);
  /// \brief Sets the color of the object
  /// \param r New red
  /// \param g New green
  /// \param b New blue
  /// \param a New alpha
  void Color(int r, int g, int b, int a);
  /// \brief Gets the fill value of the object
  /// \return True if the object is filled
  ///         False if the object is outlined
  bool Fill();
  /// \brief Sets the fill value of the object
  /// \param fill If the geometry should be filled
  void SetFill(bool fill);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief Rectangle Geometry class
class Rectangle : public Geometry
{
  /// \brief Rectangle to represent
  SDL_Rect _rect;

public:
  /// \brief Constructor
  /// \param parent Parent Object creating this Object
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Rectangle(Object *parent = nullptr, std::string name = "Rectangle");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param rect Rectangle to represent
  /// \param c Color of the object
  /// \param fill Determines if the object is filled or outlined
  /// \param parent Parent Object creating this Object
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Rectangle(SDL_Rect rect, Aspen::Graphics::Color c = Aspen::Graphics::Color(), bool fill = false, Object *parent = nullptr, std::string name = "Rectangle");
  /// \brief Destructor
  ~Rectangle();

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Gets the SDL_Rect of this object
  /// \return Reference to _rect
  SDL_Rect &GetRect();

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief Point Geometry class
class Point : public Geometry
{
  /// \brief Point to represent
  SDL_Point _point;

public:
  /// \brief Constructor
  /// \param parent Parent Object creating this Object
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Point(Object *parent = nullptr, std::string name = "Point");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param point Point to represent
  /// \param c Color of the object
  /// \param parent Parent Object creating this Object
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Point(SDL_Point point, Aspen::Graphics::Color c = Aspen::Graphics::Color(), Object *parent = nullptr, std::string name = "Point");
  /// \brief Destructor
  ~Point();

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Gets the SDL_Point of this object
  /// \return Reference to _point
  SDL_Point &GetPoint();

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief Line Geometry class
class Line : public Geometry
{
  /// \brief Starting point of the line
  SDL_Point _start;
  /// \brief Ending point of the line
  SDL_Point _end;
  /// \brief Center bias of the line
  ///        Range: (0, 1)
  ///        Center point: _start * _center + _end * (1 - _center)
  float _center;

public:
  /// \brief Constructor
  /// \param parent Parent Object creating this Object
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Line(Object *parent = nullptr, std::string name = "Line");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param start Starting point of the line
  /// \param end Ending point of the line
  /// \param center Center bias
  /// \param c Color of the object
  /// \param parent Parent Object creating this Object
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Line(SDL_Point start, SDL_Point end, float center = 0.5f, Aspen::Graphics::Color c = Aspen::Graphics::Color(), Object *parent = nullptr, std::string name = "Line");
  /// \brief Destructor
  ~Line();

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Gets the starting point of this object
  /// \return Reference to _start
  SDL_Point &GetStart();
  /// \brief Gets the ending point of this object
  /// \return Reference to _end
  SDL_Point &GetEnd();
  /// \brief Gets the center bias of this object
  /// \return Reference to _center
  float &GetCenter();
  /// \brief Sets the center bias of this object
  /// \param center New center bias
  ///               Range: (0, 1)
  void SetCenter(float center);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
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
  /// \brief Rectangle to draw _tex with
  SDL_Rect _rect;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Sprite(Object *parent = nullptr, std::string name = "Sprite");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param path Path of file to load
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Sprite(std::string path, Object *parent = nullptr, std::string name = "Sprite");
  /// \brief Destructor
  ~Sprite();

  /// \brief Shuts down and invalidates Object and all of its children
  ///        An invalid child Object will be deleted by their parent after they update
  void End();

  /// \brief Draws the sprite to the parent Object's window if parent is of type Graphics
  void operator()();

  /// \brief Wrapper for Object::SetParent
  /// \param parent New parent
  void SetParent(Object *parent);

  /// \brief Generates _tex and _rect when loading the texture from _path
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

  /// \brief Gets the rectangle _tex is drawn to
  /// \return Reference to _rect
  SDL_Rect &GetRect();

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief UniformSpritesheet extension of Sprite class
///        Splits a Spritesheet into multiple sprites
class UniformSpritesheet : public Sprite
{
  /// \brief Individual frame of the spritesheet
  SDL_Rect _frame;
  /// \brief Total number of frames of the spritesheet
  int _framecount;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  UniformSpritesheet(Object *parent = nullptr, std::string name = "UniformSpritesheet");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param path Path of file to load
  /// \param frameCount Total number of frames in the animation
  ///                   Frame size is derived via this number
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  UniformSpritesheet(std::string path, unsigned frameCount, Object *parent = nullptr, std::string name = "UniformSpritesheet");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param path Path of file to load
  /// \param frameWidth Width of each individual frame of the animation
  /// \param frameHeight Height of each individual frame of the animation
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  UniformSpritesheet(std::string path, unsigned frameWidth, unsigned frameHeight, Object *parent = nullptr, std::string name = "UniformSpritesheet");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param path Path of file to load
  /// \param frameWidth Width of each individual frame of the animation
  /// \param frameHeight Height of each individual frame of the animation
  /// \param frameCount Total number of frames in the animation
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  UniformSpritesheet(std::string path, unsigned frameWidth, unsigned frameHeight, unsigned frameCount, Object *parent = nullptr, std::string name = "UniformSpritesheet");
  /// \brief Destructor
  ~UniformSpritesheet();

  /// \brief Draws the sprite to the parent Object's window if parent is of type Graphics
  void operator()();

  /// \brief Gets the total number of frames
  /// \return Total number of frames
  int GetFrameCount();
  /// \brief Gets the clipping rectangle of the spritesheet at the given frame
  /// \param frame Frame of which to get the clipping rectangle
  /// \return Clipping rectangle of the requested frame
  SDL_Rect GetClipRectangle(int frame);

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief Animation class
///        Gets all animation frames from child Sprite and UniformSpritesheet Objects
class Animation : public Object::Object
{
  /// \brief Current frame of the animation
  int _currentFrame;
  /// \brief Delay in seconds between each frame
  float _delay;
  /// \brief Delay in seconds before the next frame change
  float _remainingDelay;
  /// \brief True if the animation just looped
  bool _done;

public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Animation(Object *parent = nullptr, std::string name = "Animation");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param spritesheet UniformSpritesheet to use as a child and from which to get frames
  /// \param frameDelay Delay in seconds between each frame
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Animation(UniformSpritesheet *spritesheet, float frameDelay = 0.05f, Object *parent = nullptr, std::string name = "Animation");
  /// \brief Destructor
  ~Animation();

  /// \brief Draws the sprite to the parent Object's window if parent is of type Graphics
  void operator()();

  /// \brief Gets the total number of frames
  ///        Calculates this by combining the frames of all Sprite and UniformSpritesheet children
  /// \return Total number of frames
  int GetFrameCount();

  /// \brief Gets the current frame
  /// \return Current frame
  int GetFrame();
  /// \brief Sets the current frame
  /// \param frame New frame
  void SetFrame(int frame);

  /// \brief Gets the current frame delay
  /// \return Current frame delay
  float GetFrameDelay();
  /// \brief Sets the frame delay
  /// \param frame New frame delay
  void SetFrameDelay(float delay);

  /// \brief True if the animation just looped
  bool Done();

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};

/// \brief FontCache class
///        Caches loaded fonts for drawing text with
class FontCache : public Object::Object
{
  /// \brief Map of Font names to paths
  std::map<std::string, std::string> _paths;
  /// \brief Map of (Font name, Font size) to loaded fonts
  std::map<std::pair<std::string, int>, TTF_Font *> _fonts;

public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  FontCache(Object *parent = nullptr, std::string name = "FontCache");
  /// \brief Destructor
  ~FontCache();

  /// \brief Adds the requested font to the list of recognized paths
  /// \param path Path to add to the recognized list
  void LoadFont(std::string path);
  /// \brief Adds the requested font to the list of recognized paths under the given name
  /// \param path Path to add to the recognized list
  /// \param name Font name to recognize the path as
  void LoadFont(std::string path, std::string name);
  /// \brief Removes the font from the recognized list and unloads all cached fonts by that name
  /// \param name Font name to unload
  void UnloadFont(std::string name);
  /// \brief Gets a cached font
  ///        Generates a new TTF_Font if it isn't yet cached
  /// \param name Name of font to get
  /// \param size Size of font in pt to get
  /// \return Cached font
  TTF_Font *GetFont(std::string name, int size);
};

/// \brief Forward declaration
class Graphics;

/// \brief Camera class
class Camera : public Object::Object
{
  /// \brief Graphics object currently using the Camera
  Graphics *_gfx;

public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Camera(Object *parent = nullptr, std::string name = "Camera");

  /// \brief Finds the graphics in the main expected location and sets its camera to this
  ///        Engine
  ///          +--- Graphics
  ///          +--- GameStateManager
  ///                 +--- GameState
  ///                        +--- Camera
  void SelectCamera();

  /// \brief Gets the Graphics object currently using this camera
  /// \return _gfx
  Graphics *GetGraphics();
  /// \brief Sets a given Graphics' camera to this
  /// \param gfx New Graphics
  void SetGraphics(Graphics *gfx);

  /// \brief Shuts down and invalidates Object and all of its children
  ///        An invalid child Object will be deleted by their parent after they update
  void End();
};

/// \brief Graphics class
class Graphics : public Object::Object
{
  /// \brief Number of Graphics Objects created
  ///        Used for determining if SDL should be initialized or closed
  static unsigned _gcount;
  /// \brief Window to be displayed
  SDL_Window *_window;
  /// \brief Surface of _window
  SDL_Surface *_surface;
  /// \brief Hardware accelerated renderer
  SDL_Renderer *_renderer;
  /// \brief Background color of the window
  Color _background;
  /// \brief Currently selected camera
  Camera *_camera;
  /// \brief First created Graphics object
  static Graphics *_main;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Graphics(Object *parent = nullptr, std::string name = "Graphics");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param w Width of window
  /// \param h Height of window
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  Graphics(int w, int h, Object *parent = nullptr, std::string name = "Graphics");
  /// \brief Destructor
  ~Graphics();

  /// \brief Gets the main graphics object
  /// \return _main
  static Graphics *Get();

  /// \brief Updates this object and all of its children
  ///        Derived classes should call or reimplement this at some point in their operator()
  ///        This won't run if the Object isn't Active
  void operator()();

  /// \brief Starts the frame
  void OnEarlyUpdate();
  /// \brief Ends the frame
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
  /// \brief Gets the hardware accelerated renderer
  /// \return _renderer
  SDL_Renderer *GetRenderer();

  /// \brief Draws a given Rectangle
  /// \param rect Rectangle to draw
  void DrawRectangle(Rectangle *rect);
  /// \brief Draws a given Rectangle
  /// \param rect Rectangle to draw
  /// \param c Color to draw in
  /// \param fill Determines if the rectangle is filled or outlined
  void DrawRectangle(SDL_Rect *rect, Color c, bool fill = true);

  /// \brief Draws a given point
  /// \param point Point to draw
  void DrawPoint(Point *point);
  /// \brief Draws a given point
  /// \param point Point to draw
  /// \param c Color to draw in
  void DrawPoint(SDL_Point *point, Color c);

  /// \brief Draws a given line
  /// \param line Line to draw
  void DrawLine(Line *line);
  /// \brief Draws a specified line
  /// \param start Starting point
  /// \param end Ending point
  /// \param c Color to draw in
  void DrawLine(SDL_Point *start, SDL_Point *end, Color c);

  /// \brief Draws a given Sprite
  /// \param sprite Sprite to draw
  void DrawSprite(Sprite *sprite);
  /// \brief Draws a given Sprite
  /// \param sprite Sprite to draw
  /// \param clip Clipping rectangle to apply as a mask
  void DrawSprite(Sprite *sprite, SDL_Rect clip);

  /// \brief Draws text
  /// \param text Text to draw
  void DrawText(UI::Text *text);
  /// \brief Draws text
  /// \param text Text to draw
  /// \param clip Clipping rectangle to apply as a mask
  void DrawText(UI::Text *text, SDL_Rect clip);

  /// \brief Sets the current camera
  /// \param camera Camera to use
  void SetCamera(Camera *camera);
  /// \brief Gets the current camera
  /// \return Current camera
  Camera *GetCamera();

  /// \brief Frees the Window and shuts down SDL if this is the last Graphics object
  void End();

  /// \brief Fills out the Debugger if it exists with this Object's information
  ///        Derived classes should call their base class's version of this method
  void PopulateDebugger();
};
} // namespace Graphics
} // namespace Aspen

#endif
