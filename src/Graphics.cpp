#define __GRAPHICS_CPP

#include "Graphics.hpp"
#include "Engine.hpp"
#include "Transform.hpp"
#include "Debug.hpp"
#include "Log.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <SDL2/SDL_image.h>
#include "imgui.h"

#undef __GRAPHICS_CPP

namespace Aspen
{
namespace Graphics
{
const int DEFAULT_WINDOW_WIDTH = 640;
const int DEFAULT_WINDOW_HEIGHT = 480;

/////////////////////////////////////////////////////////

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

/////////////////////////////////////////////////////////

Geometry::Geometry(Object *parent, std::string name)
    : Geometry(0xFFFFFFFF, false, parent, name)
{
}

Geometry::Geometry(Aspen::Graphics::Color c, bool fill, Object *parent, std::string name)
    : Object(parent, name), _c(c), _fill(fill)
{
  CreateChild<Transform::Transform>();
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

void Geometry::PopulateDebugger()
{
  ImGui::Text("Red: 0x%x", _c.Red());
  ImGui::Text("Green: 0x%x", _c.Green());
  ImGui::Text("Blue: 0x%x", _c.Blue());
  ImGui::Text("Alpha: 0x%x", _c.Alpha());
  ImGui::Text("Fill: %s", _fill ? "True" : "False");
  Object::PopulateDebugger();
}

/////////////////////////////////////////////////////////

Rectangle::Rectangle(Object *parent, std::string name)
    : Rectangle(SDL_Rect{0, 0, 1, 1}, Aspen::Graphics::Color(), false, parent, name)
{
}

Rectangle::Rectangle(SDL_Rect rect, Aspen::Graphics::Color c, bool fill, Object *parent, std::string name)
    : Geometry(c, fill, parent, name), _rect(rect)
{
}

Rectangle::~Rectangle()
{
}

void Rectangle::operator()()
{
  if (!Valid())
    return;
  Graphics *gfx = FindAncestorOfType<Graphics>();
  if (gfx)
    gfx->DrawRectangle(this);
  Object::operator()();
}

SDL_Rect &Rectangle::GetRect()
{
  return _rect;
}

void Rectangle::PopulateDebugger()
{
  ImGui::Text("X Pos: %d", _rect.x);
  ImGui::Text("Y Pos: %d", _rect.y);
  ImGui::Text("Width: %d", _rect.w);
  ImGui::Text("Height: %d", _rect.h);
  Geometry::PopulateDebugger();
}

/////////////////////////////////////////////////////////

Point::Point(Object *parent, std::string name)
    : Point(SDL_Point{0, 0}, Aspen::Graphics::Color(), parent, name)
{
}

Point::Point(SDL_Point point, Aspen::Graphics::Color c, Object *parent, std::string name)
    : Geometry(c, false, parent, name), _point(point)
{
}

Point::~Point()
{
}

void Point::operator()()
{
  if (!Valid())
    return;
  Graphics *gfx = FindAncestorOfType<Graphics>();
  if (gfx)
    gfx->DrawPoint(this);
  Object::operator()();
}

SDL_Point &Point::GetPoint()
{
  return _point;
}

void Point::PopulateDebugger()
{
  ImGui::Text("X Pos: %d", _point.x);
  ImGui::Text("Y Pos: %d", _point.y);
  Geometry::PopulateDebugger();
}

/////////////////////////////////////////////////////////

Line::Line(Object *parent, std::string name)
    : Line({0, 0}, {0, 0}, 0.5f, Aspen::Graphics::Color(), parent, name)
{
}

Line::Line(SDL_Point start, SDL_Point end, float center, Aspen::Graphics::Color c, Object *parent, std::string name)
    : Geometry(c, false, parent, name), _start(start), _end(end), _center(center)
{
}

Line::~Line()
{
}

void Line::operator()()
{
  if (!Valid())
    return;
  Graphics *gfx = FindAncestorOfType<Graphics>();
  if (gfx)
    gfx->DrawLine(this);
  Object::operator()();
}

SDL_Point &Line::GetStart()
{
  return _start;
}

SDL_Point &Line::GetEnd()
{
  return _end;
}

float &Line::GetCenter()
{
  return _center;
}

void Line::SetCenter(float center)
{
  _center = center;
}

void Line::PopulateDebugger()
{
  ImGui::Text("Start X Pos: %d", _start.x);
  ImGui::Text("Start Y Pos: %d", _start.y);
  ImGui::Text("End X Pos: %d", _end.x);
  ImGui::Text("End Y Pos: %d", _end.y);
  ImGui::Text("Center: %f", _center);
  Geometry::PopulateDebugger();
}

/////////////////////////////////////////////////////////

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

  std::vector<Debug::Debug *> debuggers;
  for (Object *child : _children)
  {
    if (dynamic_cast<Debug::Debug *>(child))
      debuggers.push_back(dynamic_cast<Debug::Debug *>(child));
    else
    {
      (*child)();
      if (!child->Valid())
      {
        RemoveChild(child);
        delete child;
      }
    }
  }
  for (Debug::Debug *debugger : debuggers)
  {
    (*debugger)();
    if (!debugger->Valid())
    {
      RemoveChild(debugger);
      delete debugger;
    }
  }

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
    SDL_Rect rectangle = rect->GetRect();
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

void Graphics::DrawPoint(Point *point)
{
  if (point)
  {
    SDL_SetRenderDrawColor(_renderer, point->Color().Red(), point->Color().Green(), point->Color().Blue(), point->Color().Alpha());
    Transform::Transform *tf = point->FindChildOfType<Transform::Transform>();
    SDL_Point p = point->GetPoint();
    if (tf)
    {
      p.x += tf->GetXPosition();
      p.y += tf->GetYPosition();
    }
    SDL_RenderDrawPoint(_renderer, p.x, p.y);
  }
}

void Graphics::DrawPoint(SDL_Point *point, Color c)
{
  if (point)
  {
    SDL_SetRenderDrawColor(_renderer, c.Red(), c.Green(), c.Blue(), c.Alpha());
    SDL_RenderDrawPoint(_renderer, point->x, point->y);
  }
}

void Graphics::DrawLine(Line *line)
{
  if (line)
  {
    SDL_SetRenderDrawColor(_renderer, line->Color().Red(), line->Color().Green(), line->Color().Blue(), line->Color().Alpha());
    Transform::Transform *tf = line->FindChildOfType<Transform::Transform>();
    SDL_Point start = line->GetStart();
    SDL_Point end = line->GetEnd();
    if (tf)
    {
      double angle = tf->GetRotation();
      SDL_Point center = {int(end.x * line->GetCenter() + start.x * (1.0f - line->GetCenter())),
                          int(end.y * line->GetCenter() + start.y * (1.0f - line->GetCenter()))};
      SDL_Point ds = {int((start.x - center.x) * tf->GetXScale()),
                      int((start.y - center.y) * tf->GetYScale())};
      SDL_Point de = {int((end.x - center.x) * tf->GetXScale()),
                      int((end.y - center.y) * tf->GetYScale())};
      ds = {int(ds.x * std::cos(angle) - ds.y * std::sin(angle)),
            int(ds.x * std::sin(angle) + ds.y * std::cos(angle))};
      de = {int(de.x * std::cos(angle) - de.y * std::sin(angle)),
            int(de.x * std::sin(angle) + de.y * std::cos(angle))};
      start.x = ds.x + tf->GetXPosition();
      start.y = ds.y + tf->GetYPosition();
      end.x = de.x + tf->GetXPosition();
      end.y = de.y + tf->GetYPosition();
    }
    SDL_RenderDrawLine(_renderer, start.x, start.y, end.x, end.y);
  }
}

void Graphics::DrawLine(SDL_Point *start, SDL_Point *end, Color c)
{
  if (start && end)
  {
    SDL_SetRenderDrawColor(_renderer, c.Red(), c.Green(), c.Blue(), c.Alpha());
    SDL_RenderDrawLine(_renderer, start->x, start->y, end->x, end->y);
  }
}

void Graphics::DrawSprite(Sprite *sprite)
{
  if (sprite && sprite->GetTexture())
  {
    SDL_Rect rect = sprite->GetRect();
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
    if (angle == 0)
      angle = 0.00000001;
    SDL_RenderCopyEx(_renderer, sprite->GetTexture(), NULL, &rect, angle, NULL, SDL_FLIP_NONE);
  }
}

void Graphics::PopulateDebugger()
{
  ImGui::Text("Graphics count: %d", _gcount);
  ImGui::Text("Window: 0x%p", _window);
  ImGui::Text("Surface: 0x%p", _surface);
  ImGui::Text("Renderer: 0x%p", _renderer);
  ImGui::Text("Background Red: 0x%x", _background.Red());
  ImGui::Text("Background Green: 0x%x", _background.Green());
  ImGui::Text("Background Blue: 0x%x", _background.Blue());
  ImGui::Text("Background Alpha: 0x%x", _background.Alpha());
  Object::PopulateDebugger();
}

/////////////////////////////////////////////////////////

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
  GetRect().x = 0;
  GetRect().y = 0;
  GetRect().w = _surface->w;
  GetRect().h = _surface->h;
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

SDL_Rect &Sprite::GetRect()
{
  return _rect;
}

void Sprite::PopulateDebugger()
{
  ImGui::Text("Path: %s", _path.c_str());
  ImGui::Text("Surface: 0x%p", _surface);
  ImGui::Text("Texture: 0x%p", _tex);
  ImGui::Text("X Pos: %d", _rect.x);
  ImGui::Text("Y Pos: %d", _rect.y);
  ImGui::Text("Size: (%d, %d)", _rect.w, _rect.h);
  Object::PopulateDebugger();
}
} // namespace Graphics
} // namespace Aspen
