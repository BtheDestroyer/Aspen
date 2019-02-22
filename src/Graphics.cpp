#define __GRAPHICS_CPP

#include "Graphics.hpp"
#include "Engine.hpp"
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

Geometry::Geometry(std::string name, Object *parent)
    : Geometry(name, parent, 0xFF, 0xFF, 0xFF, 0xFF, false)
{
}

Geometry::Geometry(std::string name, Object *parent, int r, int g, int b, int a, bool fill)
    : Object(parent, name), _r(r), _g(g), _b(b), _a(a), _fill(fill)
{
}

Geometry::~Geometry()
{
}

void Geometry::SetColor(int r, int g, int b, int a)
{
  _r = r;
  _g = g;
  _b = b;
  _a = a;
}

int Geometry::Red()
{
  return _r;
}

int Geometry::Green()
{
  return _g;
}

int Geometry::Blue()
{
  return _b;
}

int Geometry::Alpha()
{
  return _a;
}

bool Geometry::Fill()
{
  return _fill;
}

Rectangle::Rectangle(Object *parent, SDL_Rect rect)
    : Geometry("Rectangle", parent), _rect(rect)
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

Graphics::Graphics() : Graphics(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) {}

Graphics::Graphics(int w, int h)
    : Object(nullptr, "Graphics")
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

  ++_gcount;
}
Graphics::~Graphics()
{
  End();
}

void Graphics::operator()()
{
  if (!Valid())
    return;

  SetBGColor(0xFF, 0xFF, 0xFF);

  Object::operator()();

  SDL_RenderPresent(_renderer);
}

void Graphics::End()
{
  if (!Valid())
    return;
  if (_gcount-- == 1)
  {
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
    SDL_Quit();
  }
  Object::End();
}

void Graphics::SetBGColor(int r, int g, int b)
{
  if (!Valid())
    return;
  SDL_SetRenderDrawColor(_renderer, r, g, b, 0xFF);
  SDL_RenderClear(_renderer);
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
    SDL_SetRenderDrawColor(_renderer, rect->Red(), rect->Green(), rect->Blue(), rect->Alpha());
    if (rect->Fill())
      SDL_RenderFillRect(_renderer, &rect->Rect());
    else
      SDL_RenderDrawRect(_renderer, &rect->Rect());
  }
}

void Graphics::DrawRectangle(SDL_Rect *rect, int r, int g, int b, int a, bool fill)
{
  if (rect)
  {
    SDL_SetRenderDrawColor(_renderer, r, g, b, a);
    if (fill)
      SDL_RenderFillRect(_renderer, rect);
    else
      SDL_RenderDrawRect(_renderer, rect);
  }
}

void Graphics::DrawSprite(Sprite *sprite)
{
  if (sprite && sprite->GetTexture())
    SDL_RenderCopy(_renderer, sprite->GetTexture(), NULL, &sprite->Rect());
}

Sprite::Sprite(std::string path, Object *parent)
    : Object(parent, "Sprite"), _path(path), _surface(nullptr), _tex(nullptr)
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
