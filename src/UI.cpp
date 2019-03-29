#define __UI_CPP

#include "UI.hpp"
#include "Engine.hpp"
#include "Debug.hpp"

#undef __UI_CPP

namespace Aspen
{
namespace Graphics
{
namespace UI
{
Text::Text(Object *parent, std::string name)
    : Text("default", parent, name)
{
}

Text::Text(std::string text, Object *parent, std::string name)
    : Text(text, "default", 12, parent, name)
{
}

Text::Text(std::string text, std::string font, int size, Object *parent, std::string name)
    : Text(text, font, size, Colors::BLACK, parent, name)
{
}

Text::Text(std::string text, std::string font, int size, Color c, Object *parent, std::string name)
    : Object(parent, name), _text(text), _font(font), _tex(nullptr), _c(c), _rect({0, 0, 0, 0}), _size(size)
{
  CreateChild<Aspen::Transform::Transform>();
  GenerateTexture();
}

Text::~Text()
{
  if (_tex)
  {
    SDL_DestroyTexture(_tex);
    _tex = nullptr;
  }
}

void Text::operator()()
{
  if (!Active())
    return;
  if (_tex && !_text.empty())
  {
    Engine::Engine *engine = Engine::Engine::Get();
    if (engine)
    {
      Graphics *gfx = Graphics::Get();
      if (gfx)
        gfx->DrawText(this);
      else
        Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
    }
    else
      Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
  }

  Object::operator()();
}

std::string Text::GetText()
{
  return _text;
}

void Text::SetText(std::string text)
{
  _text = text;
  GenerateTexture();
}

std::string Text::GetFont()
{
  return _font;
}

void Text::SetFont(std::string font)
{
  _font = font;
  GenerateTexture();
}

Color Text::GetColor()
{
  return _c;
}

void Text::SetColor(Color c)
{
  _c = c;
  GenerateTexture();
}

SDL_Rect Text::GetRect()
{
  return _rect;
}

int Text::GetSize()
{
  return _size;
}

void Text::SetSize(int size)
{
  _size = size;
  GenerateTexture();
}

void Text::GenerateTexture()
{
  if (_tex)
  {
    SDL_DestroyTexture(_tex);
    _tex = nullptr;
  }
  if (_font.empty())
    return;
  Engine::Engine *engine = Engine::Engine::Get();
  if (engine)
  {
    Graphics *gfx = Graphics::Get();
    if (gfx)
    {
      FontCache *fc = gfx->FindChildOfType<FontCache>();
      if (fc)
      {
        TTF_Font *f = fc->GetFont(_font, _size);
        if (f)
        {
          Color tc = _c;
          tc.Alpha(0);
          SDL_Surface *surface = TTF_RenderText_Shaded(f, _text.c_str(), _c, tc);
          if (surface)
          {
            SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, tc.Red(), tc.Green(), tc.Blue()));
            _tex = SDL_CreateTextureFromSurface(gfx->GetRenderer(), surface);
            if (_tex)
            {
              _rect.w = surface->w;
              _rect.h = surface->h;
            }
            else
              Log::Error("%s couldn't generate texture. Error: %s", Name().c_str(), SDL_GetError());
            SDL_FreeSurface(surface);
          }
          else
            Log::Error("%s couldn't generate surface. Error: %s", Name().c_str(), TTF_GetError());
        }
        else
          Log::Error("%s FontCache does not contain font: %s", Name().c_str(), _font.c_str());
      }
      else
        Log::Error("%s can't find FontCache in Graphics!", Name().c_str());
    }
    else
      Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
  }
  else
    Log::Error("%s requires an ancestor Engine with child Graphics!", Name().c_str());
}

SDL_Texture *Text::GetTexture()
{
  return _tex;
}

void Text::PopulateDebugger()
{
  ImGui::Text("Text: %s", _text.c_str());
  Object::PopulateDebugger();
}

/////////////////////////////////////////////////////////

Button::Button(Object *parent, std::string name)
    : Button("Button", parent, name)
{
}

Button::Button(std::string text, Object *parent, std::string name)
    : Button("Button", 16, parent, name) {}

Button::Button(std::string text, int size, Object *parent, std::string name)
    : Object(parent, name)
{
  CreateChild<Transform::Transform>();
  _rectangle = CreateChild<Rectangle>();
  _rectangle->SetFill(true);
  _rectangle->GetTransform()->SetScale(0.3f, 0.3f);
  _textObj = CreateChild<Text>();
  _textObj->SetFont("default");
  _textObj->SetSize(size * 20);
  _textObj->GetTransform()->SetScale(0.3f, 0.3f);
  _collider = CreateChild<Physics::AABBCollider>();
  _collider->SetTrigger(true);
  _collider->GetTransform()->SetScale(0.3f, 0.3f);
  SetText(text);
  _rectangle->Color(0xAAAAAAFF);
}

void Button::SetText(std::string text, int size)
{
  _textObj->SetSize(size * 20);
  SetText(text);
}

void Button::SetText(std::string text, std::string font)
{
  _textObj->SetFont(font);
  SetText(text);
}

void Button::SetText(std::string text, int size, std::string font)
{
  _textObj->SetSize(size * 20);
  _textObj->SetFont(font);
  SetText(text);
}

void Button::SetText(std::string text)
{
  if (_text == text)
    return;
  _text = text;
  _textObj->SetText(_text);
  _rectangle->GetRect().w = _textObj->GetRect().w + 40;
  _rectangle->GetRect().h = _textObj->GetRect().h + 20;
  _collider->SetSize(_rectangle->GetRect().w * _rectangle->GetTransform()->GetLocalXScale(),
                     _rectangle->GetRect().h * _rectangle->GetTransform()->GetLocalYScale());
}

void Button::SetOnClick(ButtonCallback cb)
{
  _onClick = cb;
}

void Button::OnMouseEnter()
{
  _rectangle->Color(0x777777FF);
}

void Button::OnMouseExit()
{
  _rectangle->Color(0xAAAAAAFF);
}

void Button::OnMouseClick()
{
  if (Input::GetMouse().left.pressed && _onClick)
    _onClick(this);
}

} // namespace UI
} // namespace Graphics
} // namespace Aspen
