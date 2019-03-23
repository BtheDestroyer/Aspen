#define __GAMESTATE_CPP

#include "GameState.hpp"
#include <fstream>

#undef __GAMESTATE_CPP

namespace Aspen
{
namespace GameState
{
GameState::GameState(Object *parent, std::string name)
    : Object(parent, name)
{
  StateName(name);
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

GameState *GameStateManager::GetState(unsigned i)
{
  for (unsigned j = 0; j < _children.size() && i < j - _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs && i-- == 0)
      return gs;
  }
  return nullptr;
}

GameState *GameStateManager::GetState(std::string name)
{
  for (unsigned j = 0; j < _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs && gs->StateName() == name)
      return gs;
  }
  return nullptr;
}

void GameStateManager::UnloadAllStates()
{
  for (unsigned j = 0; j < _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs)
      gs->End();
  }
}

void GameStateManager::UnloadState(unsigned i)
{
  for (unsigned j = 0; j < _children.size() && i < j - _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs && i-- == 0)
      gs->End();
  }
}

void GameStateManager::UnloadState(GameState *state)
{
  for (unsigned j = 0; j < _children.size(); ++j)
    if (_children[j] == state)
      return _children[j]->End();
}

void GameStateManager::UnloadState(std::string name)
{
  for (unsigned j = 0; j < _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs && gs->StateName() == name)
      gs->End();
  }
}

bool GameStateManager::SetCurrentState(unsigned i)
{
  bool success = false;
  for (unsigned j = 0; j < _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs)
    {
      if (i-- == 0)
      {
        success = true;
        gs->Activate();
      }
      else
        gs->Deactivate();
    }
  }
  return success;
}

bool GameStateManager::SetCurrentState(GameState *state)
{
  bool success = false;
  for (unsigned j = 0; j < _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs)
    {
      if (gs == state)
      {
        success = true;
        gs->Activate();
      }
      else
        gs->Deactivate();
    }
  }
  return success;
}

bool GameStateManager::SetCurrentState(std::string name)
{
  bool success = false;
  for (unsigned j = 0; j < _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs)
    {
      gs->Deactivate();
      if (gs->StateName() == name)
      {
        success = true;
        gs->Activate();
      }
    }
  }
  return success;
}

bool GameStateManager::ActivateState(unsigned i)
{
  for (unsigned j = 0; j < _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs)
    {
      if (i-- == 0)
      {
        gs->Activate();
        return true;
      }
    }
  }
  return false;
}

bool GameStateManager::ActivateState(GameState *state)
{
  for (unsigned j = 0; j < _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs)
    {
      if (gs == state)
      {
        gs->Activate();
        return true;
      }
    }
  }
  return false;
}

bool GameStateManager::ActivateState(std::string name)
{
  for (unsigned j = 0; j < _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs)
    {
      if (gs->StateName() == name)
      {
        gs->Activate();
        return true;
      }
    }
  }
  return false;
}

bool GameStateManager::DeactivateState(unsigned i)
{
  for (unsigned j = 0; j < _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs)
    {
      if (i-- == 0)
      {
        gs->Deactivate();
        return true;
      }
    }
  }
  return false;
}

bool GameStateManager::DeactivateState(GameState *state)
{
  for (unsigned j = 0; j < _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs)
    {
      if (gs == state)
      {
        gs->Deactivate();
        return true;
      }
    }
  }
  return false;
}

bool GameStateManager::DeactivateState(std::string name)
{
  for (unsigned j = 0; j < _children.size(); ++j)
  {
    GameState *gs = dynamic_cast<GameState *>(_children[j]);
    if (gs)
    {
      if (gs->StateName() == name)
      {
        gs->Deactivate();
        return true;
      }
    }
  }
  return false;
}
} // namespace GameState
} // namespace Aspen
