#define __GRAPHICS_CPP

#include "Graphics.hpp"
#include "Log.hpp"
#include <sstream>
#include <iomanip>

#undef __GRAPHICS_CPP

namespace Gladius
{
namespace Graphics
{
const int DEFAULT_WINDOW_WIDTH = 640;
const int DEFAULT_WINDOW_HEIGHT = 480;

Graphics::Graphics() : Graphics(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) {}

Graphics::Graphics(int w, int h) : Object()
{
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
  {
    Log::Error(std::string("Could not initialize SDL. SDL_Error: ") + SDL_GetError());
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
    Log::Error(std::string("Could not initialize window. SDL_Error: ") + SDL_GetError());
    SDL_Quit();
    _valid = false;
    return;
  }

  _surface = SDL_GetWindowSurface(_window);
  if (!_surface)
  {
    Log::Error(std::string("Could not get window surface. SDL_Error: ") + SDL_GetError());
    SDL_DestroyWindow(_window);
    _window = nullptr;
    SDL_Quit();
    _valid = false;
    return;
  }
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
  if (_window)
  {
    SDL_DestroyWindow(_window);
    _window = nullptr;
  }
  _surface = nullptr;
  SDL_Quit();
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
    : Object(parent), _path(path)
{
  if (path.substr(path.length() - 4) == ".bmp")
    _surface = SDL_LoadBMP(path.c_str());
  else
  {
    std::stringstream inf;
    inf << "Unknown extension of path: " << path;
    Log::Error(inf);
    _valid = false;
    return;
  }

  if (!_surface)
  {
    std::stringstream inf;
    inf << "Failed to load sprite!";
    Log::Error(inf);
    _valid = false;
    return;
  }
}

Sprite::~Sprite()
{
  if (!Valid())
    return;

  SDL_FreeSurface(_surface);
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
} // namespace Gladius
