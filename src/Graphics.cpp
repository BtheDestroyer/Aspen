#define __GRAPHICS_CPP

#include "Graphics.hpp"
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
    _window = SDL_CreateWindow(
        "SDL Tutorial",
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
  }
  ++_gcount;
}
Graphics::~Graphics()
{
}

void Graphics::operator()()
{
  if (!Valid())
    return;

  Object::operator()();

  SDL_UpdateWindowSurface(_window);
}

void Graphics::End()
{
  if (!Valid())
    return;
  if (_gcount-- == 1)
  {
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
  SDL_FillRect(_surface, NULL, SDL_MapRGB(_surface->format, r, g, b));
}

SDL_Surface *Graphics::GetSurface()
{
  return _surface;
}

SDL_Window *Graphics::GetWindow()
{
  return _window;
}

void Graphics::DrawSprite(Sprite *sprite)
{
  if (sprite)
    SDL_BlitSurface(sprite->GetSurface(), NULL, _surface, NULL);
}

Sprite::Sprite(std::string path, Object *parent)
    : Object(parent, "Sprite"), _path(path)
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

  OptimizeSurface();
}

Sprite::~Sprite()
{
  if (!Valid())
    return;

  SDL_FreeSurface(_surface);
}

void Sprite::SetParent(Object *parent)
{
  Object::SetParent(parent);
  OptimizeSurface();
}

void Sprite::OptimizeSurface()
{
  if (Valid() && _surface && dynamic_cast<Graphics *>(_parent))
  {
    SDL_Surface *temp = _surface;
    _surface = SDL_ConvertSurface(temp, dynamic_cast<Graphics *>(_parent)->GetSurface()->format, 0);
    if (!_surface)
    {
      Log::Error("Unable to convert surface format! Unconverted will be used. SDL_Error: %s", SDL_GetError());
      _surface = temp;
    }
    else
      SDL_FreeSurface(temp);
  }
}

void Sprite::operator()()
{
  if (_parent && dynamic_cast<Graphics *>(_parent))
    dynamic_cast<Graphics *>(_parent)->DrawSprite(this);

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

} // namespace Graphics
} // namespace Aspen
