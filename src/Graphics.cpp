#define __GRAPHICS_CPP

#include "Graphics.hpp"
#include "Engine.hpp"
#include "Transform.hpp"
#include "Log.hpp"
#include <sstream>
#include <iomanip>
#include <SDL2/SDL_image.h>

#undef __GRAPHICS_CPP

namespace Aspen
{
namespace Graphics
{
const int DEFAULT_WINDOW_WIDTH = 640;
const int DEFAULT_WINDOW_HEIGHT = 480;
Color::Color(int color)
    : _c(color)
{
}
Color::Color(int r, int g, int b, int a)
    : _c(0)
{
  Red(r);
  Green(g);
  Blue(b);
  Alpha(a);
}

int Color::Red()
{
  return (COLOR_MASK::RED & _c) >> (8 * 3);
}

int Color::Green()
{
  return (COLOR_MASK::GREEN & _c) >> (8 * 2);
}

int Color::Blue()
{
  return (COLOR_MASK::BLUE & _c) >> 8;
}

int Color::Alpha()
{
  return COLOR_MASK::ALPHA & _c;
}

void Color::Red(int r)
{
  _c |= (r << (8 * 3) | (_c & (COLOR_MASK::BLUE | COLOR_MASK::GREEN | COLOR_MASK::ALPHA)));
}

void Color::Green(int g)
{
  _c |= (g << (8 * 2) | (_c & (COLOR_MASK::RED | COLOR_MASK::BLUE | COLOR_MASK::ALPHA)));
}

void Color::Blue(int b)
{
  _c |= (b << 8 | (_c & (COLOR_MASK::RED | COLOR_MASK::GREEN | COLOR_MASK::ALPHA)));
}

void Color::Alpha(int a)
{
  _c |= (a | (_c & (COLOR_MASK::RED | COLOR_MASK::GREEN | COLOR_MASK::BLUE)));
}

Geometry::Geometry(Object *parent, std::string name)
    : Geometry(0xFFFFFFFF, false, parent, name)
{
}

Geometry::Geometry(Aspen::Graphics::Color c, bool fill, Object *parent, std::string name)
    : Object(parent, name), _c(c), _fill(fill)
{
}

Geometry::~Geometry()
{
}

Color &Geometry::Color()
{
  return _c;
}

void Geometry::Color(Aspen::Graphics::Color c)
{
  _c = c;
}

void Geometry::Color(int r, int g, int b, int a)
{
  _c.Red(r);
  _c.Green(g);
  _c.Blue(b);
  _c.Alpha(a);
}

bool Geometry::Fill()
{
  return _fill;
}

Rectangle::Rectangle(Object *parent, std::string name)
    : Rectangle(SDL_Rect{0, 0, 1, 1}, Aspen::Graphics::Color(), false, parent, name)
{
}

Rectangle::Rectangle(SDL_Rect rect, Aspen::Graphics::Color c, bool fill, Object *parent, std::string name)
    : Geometry(c, fill, parent, "Rectangle"), _rect(rect)
{
}

Rectangle::~Rectangle()
{
}

void Rectangle::operator()()
{
  if (!Valid())
    return;
  if (static_cast<Graphics *>(_parent))
    static_cast<Graphics *>(_parent)->DrawRectangle(this);
  Object::operator()();
}

SDL_Rect &Rectangle::Rect()
{
  return _rect;
}

unsigned Graphics::_gcount = 0;

Graphics::Graphics(Object *parent, std::string name)
    : Graphics(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, parent, name)
{
}

Graphics::Graphics(int w, int h, Object *parent, std::string name)
    : Object(parent, name)
{
  if (_gcount == 0)
  {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      Log::Error("Could not initialize SDL. SDL_Error: %s", SDL_GetError());
      _valid = false;
      return;
    }
    std::stringstream title;
    title << "Aspen Engine - " << Engine::version.ToString();
    _window = SDL_CreateWindow(
        title.str().c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        w,
        h,
        SDL_WINDOW_SHOWN);
    if (!_window)
    {
      Log::Error("Could not initialize window. SDL_Error: %s", SDL_GetError());
      SDL_Quit();
      _valid = false;
      return;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
      Log::Error("Could not initialize SDL_Image. IMG_Error: %s", IMG_GetError());
      SDL_DestroyWindow(_window);
      _window = nullptr;
      SDL_Quit();
      _valid = false;
      return;
    }

    _surface = SDL_GetWindowSurface(_window);
    if (!_surface)
    {
      Log::Error("Could not get window surface. SDL_Error: %s", SDL_GetError());
      SDL_DestroyWindow(_window);
      _window = nullptr;
      SDL_Quit();
      _valid = false;
      return;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer)
    {
      Log::Error("Could not create renderer. SDL_Error: %s", SDL_GetError());
      SDL_FreeSurface(_surface);
      _surface = nullptr;
      SDL_DestroyWindow(_window);
      _window = nullptr;
      SDL_Quit();
      _valid = false;
      return;
    }
    SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
  }
  else
  {
    std::stringstream title;
    title << "Aspen Engine - " << Engine::version.ToString();
    _window = SDL_CreateWindow(
        title.str().c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        w,
        h,
        SDL_WINDOW_SHOWN);
    if (!_window)
    {
      Log::Error("Could not initialize window. SDL_Error: %s", SDL_GetError());
      _valid = false;
      return;
    }

    _surface = SDL_GetWindowSurface(_window);
    if (!_surface)
    {
      Log::Error("Could not get window surface. SDL_Error: %s", SDL_GetError());
      SDL_FreeSurface(_surface);
      _surface = nullptr;
      SDL_DestroyWindow(_window);
      _window = nullptr;
      _valid = false;
      return;
    }

    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    if (!_renderer)
    {
      Log::Error("Could not create renderer. SDL_Error: %s", SDL_GetError());
      SDL_FreeSurface(_surface);
      _surface = nullptr;
      SDL_DestroyWindow(_window);
      _window = nullptr;
      _valid = false;
      return;
    }
  }
  SetBGColor(0xFF, 0xFF, 0xFF);
  ++_gcount;
}

Graphics::~Graphics()
{
  for (Object *child : _children)
    delete child;
  _children.clear();
  End();
}

void Graphics::operator()()
{
  if (!Valid())
    return;

  SDL_SetRenderDrawColor(_renderer, _background.Red(), _background.Green(), _background.Blue(), _background.Alpha());
  SDL_RenderClear(_renderer);

  Object::operator()();

  SDL_RenderPresent(_renderer);
}

void Graphics::End()
{
  if (!Valid())
    return;
  if (_renderer)
  {
    SDL_DestroyRenderer(_renderer);
    _renderer = nullptr;
  }
  if (_window)
  {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
  _surface = nullptr;
  if (_gcount-- == 1)
  {
    SDL_Quit();
  }
  Object::End();
}

void Graphics::SetBGColor(int r, int g, int b)
{
  if (!Valid())
    return;
  _background = Color(r, g, b);
}

SDL_Surface *Graphics::GetSurface()
{
  return _surface;
}

SDL_Window *Graphics::GetWindow()
{
  return _window;
}

SDL_Renderer *Graphics::GetRenderer()
{
  return _renderer;
}

void Graphics::DrawRectangle(Rectangle *rect)
{
  if (rect)
  {
    SDL_SetRenderDrawColor(_renderer, rect->Color().Red(), rect->Color().Green(), rect->Color().Blue(), rect->Color().Alpha());
    Transform::Transform *tf = rect->FindChildOfType<Transform::Transform>();
    SDL_Rect rectangle = rect->Rect();
    if (tf)
    {
      rectangle.x += tf->GetXPosition();
      rectangle.y += tf->GetYPosition();
      rectangle.w += tf->GetXScale();
      rectangle.h += tf->GetYScale();
    }
    if (rect->Fill())
      SDL_RenderFillRect(_renderer, &rectangle);
    else
      SDL_RenderDrawRect(_renderer, &rectangle);
  }
}

void Graphics::DrawRectangle(SDL_Rect *rect, Aspen::Graphics::Color c, bool fill)
{
  if (rect)
  {
    SDL_SetRenderDrawColor(_renderer, c.Red(), c.Green(), c.Blue(), c.Alpha());
    if (fill)
      SDL_RenderFillRect(_renderer, rect);
    else
      SDL_RenderDrawRect(_renderer, rect);
  }
}

void Graphics::DrawSprite(Sprite *sprite)
{
  if (sprite && sprite->GetTexture())
  {
    SDL_Rect rect = sprite->Rect();
    double angle = 0.0;
    Transform::Transform *tf = sprite->FindChildOfType<Transform::Transform>();
    if (tf)
    {
      rect.w *= tf->GetXScale();
      rect.h *= tf->GetYScale();
      rect.x += tf->GetXPosition();
      rect.y += tf->GetYPosition();
      angle += double(tf->GetRotation());
    }
    SDL_RenderCopyEx(_renderer, sprite->GetTexture(), NULL, &rect, angle, NULL, SDL_FLIP_NONE);
  }
}

Sprite::Sprite(std::string path, Object *parent, std::string name)
    : Object(parent, name), _path(path), _surface(nullptr), _tex(nullptr)
{
  if (path.substr(path.length() - 4) == ".bmp")
  {
    _surface = SDL_LoadBMP(path.c_str());
    if (!_surface)
    {
      Log::Error("Unable to load BMP. SDL_Error: %s", SDL_GetError());
      _valid = false;
      return;
    }
  }
  else if (path.substr(path.length() - 4) == ".png")
  {
    _surface = IMG_Load(path.c_str());
    if (!_surface)
    {
      Log::Error("Unable to load PNG. IMG_Error: %s", IMG_GetError());
      _valid = false;
      return;
    }
  }
  else
  {
    Log::Error("Unknown extension of path: %s", path);
    _valid = false;
    return;
  }
  Rect().x = 0;
  Rect().y = 0;
  Rect().w = _surface->w;
  Rect().h = _surface->h;
  GenerateTexture();
  CreateChild<Transform::Transform>();
}

Sprite::~Sprite()
{
  End();
}

void Sprite::End()
{
  if (_surface)
  {
    SDL_FreeSurface(_surface);
    _surface = nullptr;
  }
  if (_tex)
  {
    SDL_DestroyTexture(_tex);
    _tex = nullptr;
  }
  Object::End();
}

void Sprite::SetParent(Object *parent)
{
  Object::SetParent(parent);
  GenerateTexture();
}

void Sprite::GenerateTexture()
{
  if (Valid() && _surface && FindAncestorOfType<Graphics>())
  {
    if (_tex)
      SDL_DestroyTexture(_tex);
    _tex = SDL_CreateTextureFromSurface(FindAncestorOfType<Graphics>()->GetRenderer(), _surface);
    if (!_tex)
    {
      Log::Error("Unable to generate texture! SDL_Error: %s", SDL_GetError());
      return;
    }
    SDL_SetTextureBlendMode(_tex, SDL_BLENDMODE_BLEND);
  }
}

void Sprite::operator()()
{
  if (Valid() && _parent && FindAncestorOfType<Graphics>())
    FindAncestorOfType<Graphics>()->DrawSprite(this);

  Object::operator()();
}
const std::string &Sprite::GetPath() const
{
  return _path;
}

SDL_Surface *Sprite::GetSurface()
{
  return _surface;
}
SDL_Texture *Sprite::GetTexture()
{
  return _tex;
}

SDL_Rect &Sprite::Rect()
{
  return _rect;
}
} // namespace Graphics
} // namespace Aspen
