#ifndef __GAMESTATE_HPP
#define __GAMESTATE_HPP

#include "Object.hpp"

/// \brief Aspen engine namespace
namespace Aspen
{
/// \brief GameState namespace
namespace GameState
{
/// \brief GameState base class
///        Inherit this to create your own states
class GameState : public Object::Object
{
  /// \brief State name
  std::string _state;

public:
  /// \brief Constructor
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  GameState(Object *parent = nullptr, std::string name = "GameState");
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param state State name
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  GameState(std::string state, Object *parent = nullptr, std::string name = "GameState");

  /// \brief Gets the state's name
  /// \return State name
  const std::string StateName();
  /// \brief Sets the state's name
  /// \param state New state name
  void StateName(std::string state);
};

/// \brief GameStateManager class
class GameStateManager : public Object::Object
{
public:
  /// \brief Constructor
  ///        Derived classes should call this in their constructors' initialization list
  /// \param parent Parent Object to be passed to Object constructor
  /// \param name Object name
  ///             Set by derived classes to a string representation of their type
  GameStateManager(Object *parent = nullptr, std::string name = "GameStateManager");

  /// \brief Loads the given GameState derived class as a new state
  /// \tparam T derived GameState
  /// \param active Determines if the state is loaded active
  /// \return Loaded state
  ///         nullptr if no state was loaded
  template <typename T>
  GameState *LoadState(bool active = false)
  {
    T *temp = new T(this);
    GameState *state = dynamic_cast<GameState *>(temp);
    if (state)
    {
      AddChild(state);
      state->SetActive(active);
    }
    else
      if (temp)
        delete temp;
    return state;
  }

  /// \brief Gets the state at the given index
  /// \param i Index to get
  /// \return GameState child at the given index
  GameState *GetState(unsigned i);
  /// \brief Gets the state with the given state name
  /// \param name State name to get
  /// \return GameState child with the given state name
  GameState *GetState(std::string name);

  /// \brief Unloads all child states
  void UnloadAllStates();
  /// \brief Unloads the state with the given index
  /// \param i Index to unload
  void UnloadState(unsigned i);
  /// \brief Unloads the given state
  /// \param state State to unload
  void UnloadState(GameState *state);
  /// \brief Unloads the state with the given state name
  /// \param name State name to unload
  void UnloadState(std::string name);

  /// \brief Sets the current state to the one at the given index
  ///        Deactivates all other states
  /// \param i Index to select
  bool SetCurrentState(unsigned i);
  /// \brief Sets the current state to the one provided
  ///        Deactivates all other states
  /// \param state State to select
  bool SetCurrentState(GameState *state);
  /// \brief Sets the current state to the one with the given state name
  ///        Deactivates all other states
  /// \param name State name to select
  bool SetCurrentState(std::string name);

  /// \brief Activates the current state to the one at the given index
  /// \param i Index to activate
  bool ActivateState(unsigned i);
  /// \brief Activates the current state to the one provided
  /// \param state State to activate
  bool ActivateState(GameState *state);
  /// \brief Activates the current state to the one with the given state name
  /// \param name State name to activate
  bool ActivateState(std::string name);

  /// \brief Deactivates the current state to the one at the given index
  /// \param i Index to deactivate
  bool DeactivateState(unsigned i);
  /// \brief Deactivates the current state to the one provided
  /// \param state State to deactivate
  bool DeactivateState(GameState *state);
  /// \brief Deactivates the current state to the one with the given state name
  /// \param name State name to deactivate
  bool DeactivateState(std::string name);
};
} // namespace GameState
} // namespace Aspen

#endif
