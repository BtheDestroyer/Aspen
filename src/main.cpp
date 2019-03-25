#include "Engine.hpp"
#include "Graphics.hpp"
#include "Transform.hpp"
#include "Controller.hpp"
#include "Physics.hpp"
#include "Time.hpp"
#include "Log.hpp"
#include "Audio.hpp"
#include "Input.hpp"
#include "GameState.hpp"
#include <functional>
#include <iostream>

using Aspen::Engine::Engine;
using Aspen::GameState::GameState;
using Aspen::GameState::GameStateManager;
using Aspen::Graphics::Graphics;
using Aspen::Object::Object;

namespace UI
{
class Button;
typedef std::function<void(Button *)> ButtonCallback;

class Button : public Object
{
  std::string _text;
  Aspen::Graphics::Text *_textObj;
  Aspen::Physics::AABBCollider *_collider;
  Aspen::Graphics::Rectangle *_rectangle;
  ButtonCallback _onClick;

public:
  Button(Object *parent = nullptr, std::string name = "Button")
      : Button("Button", parent, name)
  {
  }

  Button(std::string text, Object *parent = nullptr, std::string name = "Button")
      : Button("Button", 16, parent, name) {}
  Button(std::string text, int size, Object *parent = nullptr, std::string name = "Button")
      : Object(parent, name)
  {
    CreateChild<Aspen::Transform::Transform>();
    _rectangle = CreateChild<Aspen::Graphics::Rectangle>();
    _rectangle->SetFill(true);
    _rectangle->GetTransform()->SetScale(0.3f, 0.3f);
    _textObj = CreateChild<Aspen::Graphics::Text>();
    _textObj->SetFont("default");
    _textObj->SetSize(size * 20);
    _textObj->GetTransform()->SetScale(0.3f, 0.3f);
    _collider = CreateChild<Aspen::Physics::AABBCollider>();
    _collider->GetTransform()->SetScale(0.3f, 0.3f);
    SetText(text);
    _rectangle->Color(0xAAAAAAFF);
  }

  void SetText(std::string text, int size)
  {
    _textObj->SetSize(size * 20);
    SetText(text);
  }

  void SetText(std::string text, std::string font)
  {
    _textObj->SetFont(font);
    SetText(text);
  }

  void SetText(std::string text, int size, std::string font)
  {
    _textObj->SetSize(size * 20);
    _textObj->SetFont(font);
    SetText(text);
  }

  void SetText(std::string text)
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

  void SetOnClick(ButtonCallback cb)
  {
    _onClick = cb;
  }

  void OnMouseEnter()
  {
    _rectangle->Color(0x777777FF);
  }

  void OnMouseExit()
  {
    _rectangle->Color(0xAAAAAAFF);
  }

  void OnMouseClick()
  {
    if (Aspen::Input::GetMouse().left.pressed && _onClick)
      _onClick(this);
  }
};
} // namespace UI

void ChangeScene(UI::Button *button, std::string scene, GameStateManager *gsm)
{
  gsm->SetCurrentState(scene);
}

class MainMenu : public GameState
{
  Aspen::Audio::Music *music;
  std::vector<std::vector<unsigned>> tiles;

public:
  MainMenu(Object *parent = nullptr, std::string name = "MainMenu")
      : GameState(parent, name)
  {
    music = new Aspen::Audio::Music("resources/mus.ogg", this);
    AddChild(music);

    Aspen::Graphics::Text *title = new Aspen::Graphics::Text("Notris", "default", 64, this, "Title");
    title->GetTransform()->SetPosition(Aspen::Graphics::DEFAULT_WINDOW_WIDTH * 0.5f,
                                       Aspen::Graphics::DEFAULT_WINDOW_HEIGHT * 0.25f);
    AddChild(title);
    UI::Button *startbutton = CreateChild<UI::Button>();
    startbutton->SetText("Start");
    startbutton->GetTransform()->SetPosition(Aspen::Graphics::DEFAULT_WINDOW_WIDTH * 0.5f,
                                             Aspen::Graphics::DEFAULT_WINDOW_HEIGHT * 0.5f);
    Engine *e = FindAncestorOfType<Engine>();
    if (e)
    {
      GameStateManager *gsm = e->FindChildOfType<GameStateManager>();
      if (gsm)
        startbutton->SetOnClick(std::bind(ChangeScene, std::placeholders::_1, "Game", gsm));
    }
  }

  void OnActivate()
  {
    music->Play(true, 2.0);
  }
};

class Tile : public Object
{
  int _currentTile;

public:
  const int &currentTile;

  Tile(Object *parent = nullptr, std::string name = "Tile")
      : Object(parent, name), _currentTile(0), currentTile(_currentTile)
  {
    CreateChild<Aspen::Transform::Transform>();
    AddChild(new Aspen::Graphics::Sprite("resources/tile.grey.png", this, "Grey tile"));
    Aspen::Graphics::Sprite *newtile = new Aspen::Graphics::Sprite("resources/tile.green.png", this, "Green tile");
    newtile->Deactivate();
    AddChild(newtile);
    newtile = new Aspen::Graphics::Sprite("resources/tile.yellow.png", this, "Yellow tile");
    newtile->Deactivate();
    AddChild(newtile);
    newtile = new Aspen::Graphics::Sprite("resources/tile.blue.png", this, "Blue tile");
    newtile->Deactivate();
    AddChild(newtile);
    newtile = new Aspen::Graphics::Sprite("resources/tile.orange.png", this, "Orange tile");
    newtile->Deactivate();
    AddChild(newtile);
    newtile = new Aspen::Graphics::Sprite("resources/tile.lightblue.png", this, "Light blue tile");
    newtile->Deactivate();
    AddChild(newtile);
    newtile = new Aspen::Graphics::Sprite("resources/tile.red.png", this, "Red tile");
    newtile->Deactivate();
    AddChild(newtile);
    newtile = new Aspen::Graphics::Sprite("resources/tile.pink.png", this, "Pink tile");
    newtile->Deactivate();
    AddChild(newtile);
  }

  void SelectTile(unsigned i)
  {
    i = i > 6 ? 0 : i;
    _currentTile = i;
    for (Object *c : Children())
      if (dynamic_cast<Aspen::Graphics::Sprite *>(c))
      {
        if (!i--)
          c->Activate();
        else
          c->Deactivate();
      }
  }
};

class Trimino
{
public:
  enum SHAPE
  {
    INVALID = -1,
    I,
    C,
    COUNT
  };
  SHAPE shape;
  int x;
  int y;
  int rotation;

  Trimino(SHAPE s) : shape(s), x(0), y(0), rotation(0) {}
  Trimino(SHAPE s, int X, int Y) : shape(s), x(X), y(Y), rotation(0) {}
};

class Game : public GameState
{
  Aspen::Audio::Music *music;
  Aspen::Audio::SoundEffect *sound;
  Aspen::Graphics::Text *scoreText;
  unsigned score = 0;
  Tile *grid[20][10];

  Trimino currentPiece = Trimino(Trimino::SHAPE::C);
  Aspen::Time::Time *time;
  double moveTime = 0.5f;
  double moveTimeLeft = 0.0f;
  double inputTime = 0.1f;
  double inputTimeLeft = 0.0f;

public:
  Game(Object *parent = nullptr, std::string name = "Game")
      : GameState(parent, name)
  {
    music = new Aspen::Audio::Music("resources/tetris.ogg", this);
    AddChild(music);

    sound = new Aspen::Audio::SoundEffect("resources/pop.wav", this);
    AddChild(sound);

    UI::Button *quitbutton = CreateChild<UI::Button>();
    quitbutton->SetText("Quit");
    quitbutton->GetTransform()->SetPosition(Aspen::Graphics::DEFAULT_WINDOW_WIDTH * 0.75f, Aspen::Graphics::DEFAULT_WINDOW_HEIGHT * 0.75f);
    scoreText = CreateChild<Aspen::Graphics::Text>();
    scoreText->GetTransform()->SetPosition(Aspen::Graphics::DEFAULT_WINDOW_WIDTH * 0.75f, Aspen::Graphics::DEFAULT_WINDOW_HEIGHT * 0.25f);
    scoreText->SetSize(32);
    Engine *e = FindAncestorOfType<Engine>();
    if (e)
    {
      GameStateManager *gsm = e->FindChildOfType<GameStateManager>();
      if (gsm)
        quitbutton->SetOnClick(std::bind(ChangeScene, std::placeholders::_1, "MainMenu", gsm));
    }
    Object *tiles = CreateChild<Object>("Tiles");
    tiles->CreateChild<Aspen::Transform::Transform>()->SetPosition(Aspen::Graphics::DEFAULT_WINDOW_WIDTH * 0.25f,
                                                                   Aspen::Graphics::DEFAULT_WINDOW_HEIGHT * 0.25f);
    for (int y = 0; y < 20; ++y)
      for (int x = 0; x < 10; ++x)
      {
        Tile *t = tiles->CreateChild<Tile>();
        t->GetTransform()->SetScale(0.2f, 0.2f);
        t->GetTransform()->SetPosition(x * 80, y * 80);
        grid[y][x] = t;
      }
    tiles->CreateChild<Aspen::Physics::BoxCollider>()->SetSize(tiles->FindChildOfType<Tile>()->FindChildOfType<Aspen::Graphics::Sprite>()->GetRect().w * 5,
                                                               tiles->FindChildOfType<Tile>()->FindChildOfType<Aspen::Graphics::Sprite>()->GetRect().h * 10);
    time = FindAncestorOfType<Aspen::Engine::Engine>()->FindChildOfType<Aspen::Time::Time>();
  }

  void SetScore(int s)
  {
    char buff[128];
    snprintf(buff, 128, "Score: %.8d", score = s);
    scoreText->SetText(buff);
  }

  void AddScore(int s)
  {
    SetScore(score + s);
  }

  void DrawPiece(int color)
  {
    if (currentPiece.y < 20 && currentPiece.y >= 0 && currentPiece.x < 20 && currentPiece.x >= 0)
      grid[currentPiece.y][currentPiece.x]->SelectTile(color);
    switch (currentPiece.shape)
    {
    case Trimino::SHAPE::I:
      if (currentPiece.rotation % 2)
      {
        if (currentPiece.y + 1 < 20)
          grid[currentPiece.y + 1][currentPiece.x]->SelectTile(color);
        if (currentPiece.y - 1 >= 0)
          grid[currentPiece.y - 1][currentPiece.x]->SelectTile(color);
      }
      else
      {
        if (currentPiece.x + 1 < 20)
          grid[currentPiece.y][currentPiece.x + 1]->SelectTile(color);
        if (currentPiece.x - 1 >= 0)
          grid[currentPiece.y][currentPiece.x - 1]->SelectTile(color);
      }
      break;
    case Trimino::SHAPE::C:
      if (currentPiece.rotation == 0 || currentPiece.rotation == 1)
        if (currentPiece.y - 1 >= 0)
          grid[currentPiece.y - 1][currentPiece.x]->SelectTile(color);
      if (currentPiece.rotation == 1 || currentPiece.rotation == 2)
        if (currentPiece.x + 1 < 10)
          grid[currentPiece.y][currentPiece.x + 1]->SelectTile(color);
      if (currentPiece.rotation == 2 || currentPiece.rotation == 3)
        if (currentPiece.y + 1 < 20)
          grid[currentPiece.y + 1][currentPiece.x]->SelectTile(color);
      if (currentPiece.rotation == 3 || currentPiece.rotation == 0)
        if (currentPiece.x - 1 >= 0)
          grid[currentPiece.y][currentPiece.x - 1]->SelectTile(color);
      break;
    default:
      break;
    }
  }

  bool CheckOverlap()
  {
    if (currentPiece.x >= 10 || currentPiece.x < 0 || currentPiece.y >= 20 || grid[currentPiece.y][currentPiece.x]->currentTile)
      return true;
    switch (currentPiece.shape)
    {
    case Trimino::SHAPE::I:
      if (currentPiece.rotation % 2)
      {
        if (currentPiece.y + 1 >= 20 || grid[currentPiece.y + 1][currentPiece.x]->currentTile)
          return true;
        if (currentPiece.y - 1 >= 0 && grid[currentPiece.y - 1][currentPiece.x]->currentTile)
          return true;
      }
      else
      {
        if (currentPiece.x + 1 >= 10 || grid[currentPiece.y][currentPiece.x + 1]->currentTile)
          return true;
        if (currentPiece.x - 1 < 0 || grid[currentPiece.y][currentPiece.x - 1]->currentTile)
          return true;
      }
      break;
    case Trimino::SHAPE::C:
      if (currentPiece.rotation == 0 || currentPiece.rotation == 1)
        if (currentPiece.y - 1 >= 0 && grid[currentPiece.y - 1][currentPiece.x]->currentTile)
          return true;
      if (currentPiece.rotation == 1 || currentPiece.rotation == 2)
        if (currentPiece.x + 1 >= 10 || grid[currentPiece.y][currentPiece.x + 1]->currentTile)
          return true;
      if (currentPiece.rotation == 2 || currentPiece.rotation == 3)
        if (currentPiece.y + 1 >= 20 || grid[currentPiece.y + 1][currentPiece.x]->currentTile)
          return true;
      if (currentPiece.rotation == 3 || currentPiece.rotation == 0)
        if (currentPiece.x - 1 < 0 || grid[currentPiece.y][currentPiece.x - 1]->currentTile)
          return true;
      break;
    default:
      break;
    }
    return false;
  }

  void ClearPiece()
  {
    DrawPiece(0);
  }

  bool MoveDown()
  {
    bool ret = false;
    ClearPiece();
    ++currentPiece.y;
    if (CheckOverlap())
    {
      ret = true;
      --currentPiece.y;
      DrawPiece(currentPiece.shape + 1);
      ClearLines();
      NewPiece();
    }
    DrawPiece(currentPiece.shape + 1);
    return ret;
  }

  void RotateLeft()
  {
    ClearPiece();
    currentPiece.rotation = (currentPiece.rotation + 3) % 4;
    if (CheckOverlap() && MoveRight() && MoveLeft())
    {
      ClearPiece();
      currentPiece.rotation = (currentPiece.rotation + 1) % 4;
    }
    DrawPiece(currentPiece.shape + 1);
  }

  void RotateRight()
  {
    ClearPiece();
    currentPiece.rotation = (currentPiece.rotation + 1) % 4;
    if (CheckOverlap() && MoveLeft() && MoveRight())
    {
      ClearPiece();
      currentPiece.rotation = (currentPiece.rotation + 3) % 4;
    }
    DrawPiece(currentPiece.shape + 1);
  }

  bool MoveLeft()
  {
    bool ret = false;
    ClearPiece();
    --currentPiece.x;
    if (CheckOverlap())
    {
      ret = true;
      ++currentPiece.x;
    }
    DrawPiece(currentPiece.shape + 1);
    return ret;
  }

  bool MoveRight()
  {
    bool ret = false;
    ClearPiece();
    ++currentPiece.x;
    if (CheckOverlap())
    {
      ret = true;
      --currentPiece.x;
    }
    DrawPiece(currentPiece.shape + 1);
    return ret;
  }

  void NewPiece()
  {
    moveTimeLeft = moveTime;
    currentPiece = Trimino(Trimino::SHAPE(rand() % Trimino::SHAPE::COUNT));
    currentPiece.x = 5;
    currentPiece.y = 0;
  }

  void ClearLines()
  {
    int clearedLines = 0;
    for (int y = 19; y >= 0; --y)
      for (int x = 0; x < 10; ++x)
      {
        if (grid[y][x]->currentTile == 0)
          break;
        if (x == 9)
        {
          AddScore(++clearedLines * 150);
          for (int y2 = y; y2 > 0; --y2)
            for (int x2 = 0; x2 < 10; ++x2)
              grid[y2][x2]->SelectTile(grid[y2 - 1][x2]->currentTile);
          for (int x2 = 0; x2 < 10; ++x2)
            grid[0][x2]->SelectTile(0);
          ++y;
        }
      }
  }

  void OnUpdate()
  {
    if (moveTimeLeft > 0)
      moveTimeLeft -= time->DeltaTime();
    if (inputTimeLeft > 0)
      inputTimeLeft -= time->DeltaTime();
    if (moveTimeLeft <= 0)
    {
      moveTimeLeft += moveTime;
      MoveDown();
    }
    if (Aspen::Input::KeyPressed(SDLK_q))
    {
      sound->Play();
      RotateLeft();
    }
    if (Aspen::Input::KeyPressed(SDLK_e))
    {
      sound->Play();
      RotateRight();
    }
    if (Aspen::Input::KeyHeld(SDLK_a) && inputTimeLeft <= 0)
    {
      sound->Play();
      inputTimeLeft += inputTime;
      MoveLeft();
    }
    if (Aspen::Input::KeyHeld(SDLK_d) && inputTimeLeft <= 0)
    {
      sound->Play();
      inputTimeLeft += inputTime;
      MoveRight();
    }
    if (Aspen::Input::KeyHeld(SDLK_s) && inputTimeLeft <= 0)
    {
      sound->Play();
      moveTimeLeft = moveTime;
      inputTimeLeft += inputTime;
      MoveDown();
    }
    if (Aspen::Input::KeyPressed(SDLK_w))
    {
      sound->Play();
      moveTimeLeft = moveTime;
      while (!MoveDown())
        ;
      moveTimeLeft = moveTime;
    }
  }

  void OnActivate()
  {
    music->Play(true, 0.5);
    for (int x = 0; x < 10; ++x)
      for (int y = 0; y < 20; ++y)
        grid[y][x]->SelectTile(0);
    NewPiece();
    DrawPiece(currentPiece.shape + 1);
    SetScore(0);
  }
};

int main(int argc, char **argv)
{
  Aspen::Log::Log::SetFile("./Aspen.log");

  Engine engine(Aspen::Engine::START_FLAGS::ALL);
  engine.FindChildOfType<Aspen::Physics::Physics>()->SetGravityStrength(0);
  engine.FindChildOfType<Aspen::Physics::Physics>()->SetDrag(0.1);
  engine.FindChildOfType<Aspen::Time::Time>()->TargetFramerate(60);
  engine.FindChildOfType<Aspen::Graphics::Graphics>()->FindChildOfType<Aspen::Graphics::FontCache>()->LoadFont("resources/ABeeZee-Regular.ttf", "default");

  engine.FindChildOfType<GameStateManager>()->LoadState<MainMenu>(true);
  engine.FindChildOfType<GameStateManager>()->LoadState<Game>(false);

  while (engine)
    engine();
  return 0;
}
