#ifndef __GAMESTATE_HPP
#define __GAMESTATE_HPP

#include "Object.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief GameState namespace
namespace GameState
{
class GameState : public Object::Object
{
  std::string _state;

public:
  GameState(Object *parent = nullptr, std::string name = "GameState");

  void UnloadState();

  const std::string StateName();
  void StateName(std::string state);
};

class GameStateManager : public Object::Object
{
public:
  GameStateManager(Object *parent = nullptr, std::string name = "GameStateManager");

  template <typename T>
  GameState *LoadState(bool active = true)
  {
    T *temp = new T;
    GameState *state = nullptr;
    if (dynamic_cast<GameState*>(temp))
      state = CreateChild<T>();
    delete temp;
    return state;
  }

  void GetState(unsigned i);
  void GetState(std::string name);

  void UnloadAllStates();
  void UnloadState(unsigned i);
  void UnloadState(GameState *state);
  void UnloadState(std::string name);

  bool SetCurrentState(unsigned i);
  bool SetCurrentState(GameState *state);
  bool SetCurrentState(std::string name);

  bool ActivateState(unsigned i);
  bool ActivateState(GameState *state);
  bool ActivateState(std::string name);

  bool DeactivateState(unsigned i);
  bool DeactivateState(GameState *state);
  bool DeactivateState(std::string name);
};
} // namespace GameState
} // namespace Aspen

#endif
