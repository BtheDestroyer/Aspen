#ifndef __GRAPHICS_HPP
#define __GRAPHICS_HPP
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Log.hpp"
#include "Object.hpp"

namespace Gladius
{
namespace Graphics
{
extern const int DEFAULT_WINDOW_WIDTH;
extern const int DEFAULT_WINDOW_HEIGHT;

class Sprite : public Object::Object
{
  std::string _path;
  SDL_Surface *_surface;

public:
  Sprite(std::string path, Object *parent = nullptr);
  ~Sprite();

  void operator()();

  const std::string &GetPath() const;
  SDL_Surface *GetSurface();
};

class Graphics : public Object::Object
{
  bool _valid = false;
  SDL_Window *_window = NULL;
  SDL_Surface *_surface = NULL;

public:
  Graphics();
  Graphics(int w, int h);
  ~Graphics();

  void operator()();

  void SetBGColor(int r, int g, int b);
  SDL_Surface *GetSurface();
  SDL_Window *GetWindow();
  void DrawSprite(Sprite *sprite);

  void End();
};
} // namespace Graphics
} // namespace Gladius

#endif
