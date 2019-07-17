#define __GRAPHICS_CPP

#include "Graphics.hpp"
#include "UI.hpp"
#include "Engine.hpp"
#include "Transform.hpp"
#include "Debug.hpp"
#include "Time.hpp"
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

Color::Color(unsigned color)
    : _c(color)
{
}
Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    : _c(0)
{
  Red(r);
  Green(g);
  Blue(b);
  Alpha(a);
}

Uint8 Color::Red() const
{
  return Uint8((COLOR_MASK::RED & _c) >> (8 * 3));
}

Uint8 Color::Green() const
{
  return Uint8((COLOR_MASK::GREEN & _c) >> (8 * 2));
}

Uint8 Color::Blue() const
{
  return Uint8((COLOR_MASK::BLUE & _c) >> 8);
}

Uint8 Color::Alpha() const
{
  return Uint8(COLOR_MASK::ALPHA & _c);
}

void Color::Red(Uint8 r)
{
  _c |= (r << (8 * 3) | (_c & (COLOR_MASK::BLUE | COLOR_MASK::GREEN | COLOR_MASK::ALPHA)));
}

void Color::Green(Uint8 g)
{
  _c |= (g << (8 * 2) | (_c & (COLOR_MASK::RED | COLOR_MASK::BLUE | COLOR_MASK::ALPHA)));
}

void Color::Blue(Uint8 b)
{
  _c |= (b << 8 | (_c & (COLOR_MASK::RED | COLOR_MASK::GREEN | COLOR_MASK::ALPHA)));
}

void Color::Alpha(Uint8 a)
{
  _c |= (a | (_c & (COLOR_MASK::RED | COLOR_MASK::GREEN | COLOR_MASK::BLUE)));
}

Color::operator SDL_Color() const
{
  SDL_Color c{Red(), Green(), Blue(), Alpha()};
  return c;
}

bool Color::operator==(const Color &rhs) const
{
  return _c == rhs._c;
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

void Geometry::SetFill(bool fill)
{
  _fill = fill;
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
  if (!Active())
    return;
  Engine::Engine *engine = Engine::Engine::Get();
  if (engine)
  {
    Graphics *gfx = Graphics::Get();
    if (gfx)
      gfx->DrawRectangle(this);
    else
      Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
  }
  else
    Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
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
  if (!Active())
    return;
  Engine::Engine *engine = Engine::Engine::Get();
  if (engine)
  {
    Graphics *gfx = Graphics::Get();
    if (gfx)
      gfx->DrawPoint(this);
    else
      Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
  }
  else
    Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
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
  if (!Active())
    return;
  Engine::Engine *engine = Engine::Engine::Get();
  if (engine)
  {
    Graphics *gfx = Graphics::Get();
    if (gfx)
      gfx->DrawLine(this);
    else
      Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
  }
  else
    Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
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

FontCache::FontCache(Object *parent, std::string name)
    : Object(parent, name)
{
}

FontCache::~FontCache()
{
  End();
  for (std::pair<std::pair<std::string, int>, TTF_Font *> it : _fonts)
    TTF_CloseFont(it.second);
  _fonts.clear();
}

void FontCache::LoadFont(std::string path)
{
  LoadFont(path, path);
}

void FontCache::LoadFont(std::string path, std::string name)
{
  if (_paths.find(name) == _paths.end())
    _paths[name] = path;
}

void FontCache::UnloadFont(std::string name)
{
  for (unsigned i = 0; i < _fonts.size(); ++i)
  {
    std::map<std::pair<std::string, int>, TTF_Font *>::iterator it = _fonts.begin();
    for (unsigned j = 0; j < i; ++j)
      ++it;
    if (it->first.first == name)
    {
      TTF_CloseFont(it->second);
      _fonts.erase(it);
      --i;
    }
  }
  _paths.erase(name);
}

TTF_Font *FontCache::GetFont(std::string name, int size)
{
  if (_paths.find(name) != _paths.end())
  {
    std::pair<std::string, int> index(_paths[name], size);
    if (_fonts.find(index) == _fonts.end())
    {
      TTF_Font *font = TTF_OpenFont(_paths[name].c_str(), size);
      if (font)
        _fonts[index] = font;
      else
      {
        Log::Error("%s couldn't load font from path: %s", _paths[name]);
        return nullptr;
      }
    }
    return _fonts[index];
  }
  Log::Error("%s no path for the font named: %s", name);
  return nullptr;
}

/////////////////////////////////////////////////////////

Camera::Camera(Object *parent, std::string name)
    : Object(parent, name), _gfx(nullptr)
{
  CreateChild<Transform::Transform>();
}

void Camera::SelectCamera()
{
  Engine::Engine *engine = Engine::Engine::Get();
  if (engine)
    SetGraphics(Graphics::Get());
}

Graphics *Camera::GetGraphics()
{
  return _gfx;
}

void Camera::SetGraphics(Graphics *gfx)
{
  if (_gfx == gfx)
    return;
  Graphics *ogfx = _gfx;
  _gfx = gfx;
  if (ogfx)
    ogfx->SetCamera(nullptr);
  if (gfx && gfx->GetCamera() != this)
    gfx->SetCamera(this);
}

void Camera::End()
{
  if (_gfx)
    _gfx->SetCamera(nullptr);
}

/////////////////////////////////////////////////////////

unsigned Graphics::_gcount = 0;
Graphics *Graphics::_main = nullptr;

Graphics::Graphics(Object *parent, std::string name)
    : Graphics(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, parent, name)
{
}

Graphics::Graphics(int w, int h, Object *parent, std::string name)
    : Object(parent, name), _window(nullptr), _surface(nullptr), _renderer(nullptr), _background(Color()), _camera(nullptr)
{
  if (_gcount == 0)
  {
    if (SDL_WasInit(Engine::SDL_INIT_FLAGS) != Engine::SDL_INIT_FLAGS)
    {
      Log::Error("SDL is not yet initialized! SDL_Error: %s", SDL_GetError());
      _valid = false;
      return;
    }
    std::stringstream title;
    title << "Aspen Engine - " << Engine::VERSION.ToString();
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

    if (TTF_Init() == -1)
    {
      Log::Error("Could not initialize SDL_TTF. TTF_Error: %s", TTF_GetError());
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
    title << "Aspen Engine - " << Engine::VERSION.ToString();
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
  if (!_main)
    _main = this;
  ++_gcount;
}

Graphics::~Graphics()
{
  End();
  for (Object *child : _children)
    delete child;
  _children.clear();
}

Graphics *Graphics::Get()
{
  return _main;
}

void Graphics::operator()()
{
  if (!Active())
    return;

  for (Object *child : _children)
  {
    if (!dynamic_cast<Debug::Debug *>(child))
    {
      (*child)();
      if (!child->Valid())
      {
        RemoveChild(child);
        delete child;
      }
    }
  }
}

void Graphics::OnEarlyUpdate()
{
  SDL_SetRenderDrawColor(_renderer, _background.Red(), _background.Green(), _background.Blue(), _background.Alpha());
  SDL_RenderClear(_renderer);
  Object::OnEarlyUpdate();
}

void Graphics::OnLateUpdate()
{
  for (Object *child : _children)
  {
    if (dynamic_cast<Debug::Debug *>(child))
    {
      (*child)();
      if (!child->Valid())
      {
        RemoveChild(child);
        delete child;
      }
    }
  }
  SDL_RenderPresent(_renderer);
  Object::OnLateUpdate();
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
    IMG_Quit();
  if (_main == this)
    _main = nullptr;
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
    Transform::Transform *tf = rect->GetTransform();
    SDL_Rect rectangle = rect->GetRect();
    if (tf)
    {
      if (_camera && _camera->Active())
      {
        Transform::Transform *ctf = _camera->GetTransform();
        rectangle.w *= tf->GetXScale(ctf);
        rectangle.h *= tf->GetYScale(ctf);
        rectangle.x += tf->GetXPosition(ctf);
        rectangle.y += tf->GetYPosition(ctf);
      }
      else
      {
        rectangle.w *= tf->GetXScale();
        rectangle.h *= tf->GetYScale();
        rectangle.x += tf->GetXPosition();
        rectangle.y += tf->GetYPosition();
      }
    }
    rectangle.x -= rectangle.w / 2.0f;
    rectangle.y -= rectangle.h / 2.0f;
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
    Transform::Transform *tf = point->GetTransform();
    SDL_Point p = point->GetPoint();
    if (tf)
    {
      if (_camera && _camera->Active())
      {
        Transform::Transform *ctf = _camera->GetTransform();
        p.x += tf->GetXPosition(ctf);
        p.y += tf->GetYPosition(ctf);
      }
      else
      {
        p.x += tf->GetXPosition();
        p.y += tf->GetYPosition();
      }
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
    Transform::Transform *tf = line->GetTransform();
    SDL_Point start = line->GetStart();
    SDL_Point end = line->GetEnd();
    if (tf)
    {
      if (_camera && _camera->Active())
      {
        double angle = tf->GetRotation() + _camera->GetTransform()->GetInverseRotation();
        SDL_Point center = {int(end.x * line->GetCenter() + start.x * (1.0f - line->GetCenter())),
                            int(end.y * line->GetCenter() + start.y * (1.0f - line->GetCenter()))};
        SDL_Point ds = {int((start.x - center.x) * tf->GetXScale() * _camera->GetTransform()->GetInverseXScale()),
                        int((start.y - center.y) * tf->GetYScale() * _camera->GetTransform()->GetInverseYScale())};
        SDL_Point de = {int((end.x - center.x) * tf->GetXScale() * _camera->GetTransform()->GetInverseXScale()),
                        int((end.y - center.y) * tf->GetYScale() * _camera->GetTransform()->GetInverseYScale())};
        ds = {int(ds.x * std::cos(angle) - ds.y * std::sin(angle)),
              int(ds.x * std::sin(angle) + ds.y * std::cos(angle))};
        de = {int(de.x * std::cos(angle) - de.y * std::sin(angle)),
              int(de.x * std::sin(angle) + de.y * std::cos(angle))};
        start.x = ds.x + tf->GetXPosition() + _camera->GetTransform()->GetInverseXPosition();
        start.y = ds.y + tf->GetYPosition() + _camera->GetTransform()->GetInverseYPosition();
        end.x = de.x + tf->GetXPosition() + _camera->GetTransform()->GetInverseXPosition();
        end.y = de.y + tf->GetYPosition() + _camera->GetTransform()->GetInverseYPosition();
      }
      else
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
    Transform::Transform *tf = sprite->GetTransform();
    if (tf)
    {
      if (_camera && _camera->Active())
      {
        Transform::Transform *ctf = _camera->GetTransform();
        rect.w *= tf->GetXScale(ctf);
        rect.h *= tf->GetYScale(ctf);
        rect.x += tf->GetXPosition(ctf);
        rect.y += tf->GetYPosition(ctf);
        angle += tf->GetRotation(ctf);
      }
      else
      {
        rect.w *= tf->GetXScale();
        rect.h *= tf->GetYScale();
        rect.x += tf->GetXPosition();
        rect.y += tf->GetYPosition();
        angle += tf->GetRotation();
      }
    }
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    if (angle == 0)
      angle = 0.00000001;
    SDL_RenderCopyEx(_renderer, sprite->GetTexture(), NULL, &rect, (angle / M_PI) * 180.0, NULL, SDL_FLIP_NONE);
  }
}

void Graphics::DrawSprite(Sprite *sprite, SDL_Rect clip)
{
  if (sprite && sprite->GetTexture())
  {
    SDL_Rect rect = sprite->GetRect();
    double angle = 0.0;
    Transform::Transform *tf = sprite->GetTransform();
    if (tf)
    {
      if (_camera && _camera->Active())
      {
        Transform::Transform *ctf = _camera->GetTransform();
        rect.w = clip.w * tf->GetXScale(ctf);
        rect.h = clip.h * tf->GetYScale(ctf);
        rect.x += tf->GetXPosition(ctf);
        rect.y += tf->GetYPosition(ctf);
        angle += tf->GetRotation(ctf);
      }
      else
      {
        rect.w = clip.w * tf->GetXScale();
        rect.h = clip.h * tf->GetYScale();
        rect.x += tf->GetXPosition();
        rect.y += tf->GetYPosition();
        angle += tf->GetRotation();
      }
    }
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    if (angle == 0)
      angle = 0.00000001;
    SDL_RenderCopyEx(_renderer, sprite->GetTexture(), &clip, &rect, (angle / M_PI) * 180.0, NULL, SDL_FLIP_NONE);
  }
}

void Graphics::DrawText(UI::Text *text)
{
  if (text && text->GetTexture())
  {
    SDL_Rect rect = text->GetRect();
    double angle = 0.0;
    Transform::Transform *tf = text->GetTransform();
    if (tf)
    {
      if (_camera && _camera->Active())
      {
        Transform::Transform *ctf = _camera->GetTransform();
        rect.w *= tf->GetXScale(ctf);
        rect.h *= tf->GetYScale(ctf);
        rect.x += tf->GetXPosition(ctf);
        rect.y += tf->GetYPosition(ctf);
        angle += tf->GetRotation(ctf);
      }
      else
      {
        rect.w *= tf->GetXScale();
        rect.h *= tf->GetYScale();
        rect.x += tf->GetXPosition();
        rect.y += tf->GetYPosition();
        angle += tf->GetRotation();
      }
    }
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    if (angle == 0)
      angle = 0.00000001;
    SDL_RenderCopyEx(_renderer, text->GetTexture(), NULL, &rect, (angle / M_PI) * 180.0, NULL, SDL_FLIP_NONE);
  }
}

void Graphics::DrawText(UI::Text *text, SDL_Rect clip)
{
  if (text && text->GetTexture())
  {
    SDL_Rect rect = text->GetRect();
    double angle = 0.0;
    Transform::Transform *tf = text->GetTransform();
    if (tf)
    {
      if (_camera && _camera->Active())
      {
        Transform::Transform *ctf = _camera->GetTransform();
        rect.w = clip.w * tf->GetXScale(ctf);
        rect.h = clip.h * tf->GetYScale(ctf);
        rect.x += tf->GetXPosition(ctf);
        rect.y += tf->GetYPosition(ctf);
        angle += tf->GetRotation(ctf);
      }
      else
      {
        rect.w = clip.w * tf->GetXScale();
        rect.h = clip.h * tf->GetYScale();
        rect.x += tf->GetXPosition();
        rect.y += tf->GetYPosition();
        angle += tf->GetRotation();
      }
    }
    rect.x -= rect.w / 2;
    rect.y -= rect.h / 2;
    if (angle == 0)
      angle = 0.00000001;
    SDL_RenderCopyEx(_renderer, text->GetTexture(), &clip, &rect, (angle / M_PI) * 180.0, NULL, SDL_FLIP_NONE);
  }
}

void Graphics::SetCamera(Camera *camera)
{
  if (_camera == camera)
    return;
  Camera *ocamera = _camera;
  _camera = camera;
  if (ocamera)
    ocamera->SetGraphics(nullptr);
  if (camera && camera->GetGraphics() != this)
    camera->SetGraphics(this);
}

Camera *Graphics::GetCamera()
{
  return _camera;
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
  if (Valid() && _surface)
  {
    if (_tex)
    {
      SDL_DestroyTexture(_tex);
      _tex = nullptr;
    }
    Engine::Engine *engine = Engine::Engine::Get();
    if (engine)
    {
      Graphics *gfx = Graphics::Get();
      if (gfx)
      {
        _tex = SDL_CreateTextureFromSurface(gfx->GetRenderer(), _surface);
        if (!_tex)
        {
          Log::Error("%s was unable to generate texture! SDL_Error: %s", Name().c_str(), SDL_GetError());
          return;
        }
        SDL_SetTextureBlendMode(_tex, SDL_BLENDMODE_BLEND);
      }
      else
        Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
    }
    else
      Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
  }
}

void Sprite::operator()()
{
  if (Active())
  {
    Engine::Engine *engine = Engine::Engine::Get();
    if (engine)
    {
      Graphics *gfx = Graphics::Get();
      if (gfx)
        gfx->DrawSprite(this);
      else
        Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
    }
    else
      Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
    Object::operator()();
  }
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

/////////////////////////////////////////////////////////

UniformSpritesheet::UniformSpritesheet(Object *parent, std::string name)
    : UniformSpritesheet::UniformSpritesheet("", 0, parent, name)
{
}

UniformSpritesheet::UniformSpritesheet(std::string path, unsigned frameCount, Object *parent, std::string name)
    : Sprite(path, parent, name), _frame({0, 0, 0, 0}), _framecount(frameCount)
{
  SDL_Surface *s = GetSurface();
  if (s)
  {
    _frame.w = s->w / frameCount;
    _frame.h = s->h;
  }
}

UniformSpritesheet::UniformSpritesheet(std::string path, unsigned frameWidth, unsigned frameHeight, Object *parent, std::string name)
    : UniformSpritesheet::UniformSpritesheet(path, 0, 0, 0, parent, name)
{
}

UniformSpritesheet::UniformSpritesheet(std::string path, unsigned frameWidth, unsigned frameHeight, unsigned frameCount, Object *parent, std::string name)
    : Sprite(path, parent, name), _frame({0, 0, 0, 0}), _framecount(frameCount)
{
  _frame.w = frameWidth;
  _frame.h = frameHeight;
}

UniformSpritesheet::~UniformSpritesheet()
{
}

void UniformSpritesheet::operator()()
{
  if (Active())
  {
    Engine::Engine *engine = Engine::Engine::Get();
    if (engine)
    {
      Graphics *gfx = Graphics::Get();
      if (gfx)
        gfx->DrawSprite(this, GetClipRectangle(0));
      else
        Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
    }
    else
      Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
    Object::operator()();
  }
}

int UniformSpritesheet::GetFrameCount()
{
  return _framecount;
}

SDL_Rect UniformSpritesheet::GetClipRectangle(int frame)
{
  if (frame < _framecount && _framecount >= 0)
  {
    int hframes = GetSurface()->w / _frame.w;
    return SDL_Rect{_frame.w * (frame % hframes), _frame.h * (frame / hframes), _frame.w, _frame.h};
  }
  return SDL_Rect{0, 0, 0, 0};
}

void UniformSpritesheet::PopulateDebugger()
{
  ImGui::Text("Frame width: %d", _frame.w);
  ImGui::Text("Frame height: %d", _frame.h);
  ImGui::Text("Frame count: %d", _framecount);
  Sprite::PopulateDebugger();
}

/////////////////////////////////////////////////////////

Animation::Animation(Object *parent, std::string name)
    : Animation(nullptr, 0.05f, parent, name)
{
}

Animation::Animation(UniformSpritesheet *spritesheet, float frameDelay, Object *parent, std::string name)
    : Object(parent, name), _currentFrame(0), _delay(frameDelay), _remainingDelay(_delay), _done(false)
{
  AddChild(spritesheet);
  CreateChild<Transform::Transform>();
}

Animation::~Animation()
{
}

void Animation::operator()()
{
  if (!Active())
    return;
  Engine::Engine *engine = Engine::Engine::Get();
  if (engine)
  {
    Time::Time *time = engine->FindAncestorOfType<Time::Time>();
    if (_done)
      _done = false;
    if (_delay > 0)
    {
      float dt;
      if (time)
        dt = time->DeltaTime();
      else
        dt = 1 / 60.0f;

      _remainingDelay += dt;
      while (_remainingDelay >= _delay)
      {
        _remainingDelay -= _delay;
        if (++_currentFrame >= GetFrameCount())
        {
          _currentFrame = 0;
          _done = true;
        }
      }
    }

    Graphics *gfx = Graphics::Get();
    if (gfx && _currentFrame >= 0)
    {
      int f = _currentFrame;
      for (Object *child : _children)
      {
        if (dynamic_cast<UniformSpritesheet *>(child))
        {
          if (f >= 0)
          {
            UniformSpritesheet *uss = dynamic_cast<UniformSpritesheet *>(child);
            if (f - uss->GetFrameCount() >= 0)
              f -= uss->GetFrameCount();
            else
            {
              gfx->DrawSprite(uss, uss->GetClipRectangle(f));
              f = -1;
            }
          }
        }
        else if (dynamic_cast<Sprite *>(child))
        {
          if (f >= 0)
          {
            Sprite *s = dynamic_cast<Sprite *>(child);
            if (f > 0)
              --f;
            else
            {
              gfx->DrawSprite(s);
              f = -1;
            }
          }
        }
        else
          (*child)();

        if (!child->Valid())
        {
          RemoveChild(child);
          delete child;
        }
      }
    }
    else
    {
      if (!gfx)
        Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
      else
        Log::Error("%s's current frame is less than 0! (%d)", Name().c_str(), _currentFrame);
    }
  }
  else
    Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
}

int Animation::GetFrameCount()
{
  int count = 0;
  for (Object *child : _children)
  {
    if (dynamic_cast<UniformSpritesheet *>(child))
      count += dynamic_cast<UniformSpritesheet *>(child)->GetFrameCount();
    else if (dynamic_cast<Sprite *>(child))
      ++count;
  }
  return count;
}

int Animation::GetFrame()
{
  return _currentFrame;
}

void Animation::SetFrame(int frame)
{
  _currentFrame = frame % GetFrameCount();
}

float Animation::GetFrameDelay()
{
  return _delay;
}

void Animation::SetFrameDelay(float delay)
{
  _delay = delay;
}

bool Animation::Done()
{
  return _done;
}

void Animation::PopulateDebugger()
{
  ImGui::Text("Current Frame: %d / %d", _currentFrame, GetFrameCount());
  ImGui::InputFloat("Frame Delay", &_delay);
  ImGui::Text("Time till next frame: %f", _remainingDelay);
  Object::PopulateDebugger();
}
} // namespace Graphics
} // namespace Aspen
