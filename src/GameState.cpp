#define __GAMESTATE_CPP

#include "GameState.hpp"
#include "Controller.hpp"
#include "Debug.hpp"
#include "Engine.hpp"
#include "Event.hpp"
#include "Graphics.hpp"
#include "Input.hpp"
#include "Log.hpp"
#include "Object.hpp"
#include "Physics.hpp"
#include "Time.hpp"
#include "Transform.hpp"
#include <fstream>

#undef __GAMESTATE_CPP

namespace Aspen
{
namespace GameState
{
GameState::GameState(Object *parent, std::string name)
    : Object(parent, name)
{
}

void GameState::UnloadState()
{
}

const std::string GameState::StateName()
{
  return _state;
}

void GameState::StateName(std::string state)
{
  _state = state;
}

GameStateManager::GameStateManager(Object *parent, std::string name)
    : Object(parent, name)
{
}

void GameStateManager::GetState(unsigned i)
{
}

void GameStateManager::GetState(std::string name)
{
}

void GameStateManager::UnloadAllStates()
{
}

void GameStateManager::UnloadState(unsigned i)
{
}

void GameStateManager::UnloadState(GameState *state)
{
}

void GameStateManager::UnloadState(std::string name)
{
}

bool GameStateManager::SetCurrentState(unsigned i)
{
  return false;
}

bool GameStateManager::SetCurrentState(GameState *state)
{
  return false;
}

bool GameStateManager::SetCurrentState(std::string name)
{
  return false;
}

bool GameStateManager::ActivateState(unsigned i)
{
  return false;
}

bool GameStateManager::ActivateState(GameState *state)
{
  return false;
}

bool GameStateManager::ActivateState(std::string name)
{
  return false;
}

bool GameStateManager::DeactivateState(unsigned i)
{
  return false;
}

bool GameStateManager::DeactivateState(GameState *state)
{
  return false;
}

bool GameStateManager::DeactivateState(std::string name)
{
  return false;
}

} // namespace GameState
} // namespace Aspen
