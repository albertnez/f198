#ifndef PLAYER_H
#define PLAYER_H

#include "command.h"

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Joystick.hpp>

#include <map>


class CommandQueue;

class Player {
public:
  enum Status {
    Alive,
    Dead,
  };

  enum Action {
    MoveLeft,
    MoveRight,
    MoveUp,
    MoveDown,
    MoveXJoystick,
    MoveYJoystick,
    FireXJoystick,
    FireYJoystick,
    FireLeft,
    FireRight,
    FireUp,
    FireDown,
    ActionCount,
  };

  Player();

  void handle_event(const sf::Event& event, CommandQueue& commands);
  void handle_realtime_input(CommandQueue& commands);

  void assign_key(Action action, sf::Keyboard::Key key);
  sf::Keyboard::Key get_assigned_key(Action action) const;

  void assign_joystick_id(unsigned id);
  unsigned get_assigned_id() const;

  void set_axis_threshold(float threshold);
  float get_axis_threshold() const;

  void assign_button(Action action, unsigned button);
  unsigned get_assigned_button(Action action) const;

  void assign_axis(Action action, sf::Joystick::Axis axis);
  sf::Joystick::Axis get_assigned_axis(Action action) const;

  Player::Status get_status() const;
  void set_status(Status status);

private:
  void initialize_actions();
  static bool is_realtime_action(Action action);

  std::map<sf::Keyboard::Key, Action> m_key_binding;
  // Joystick
  unsigned m_joystick_id;
  float m_axis_threshold;
  std::map<unsigned, Action> m_button_binding;
  std::map<sf::Joystick::Axis, Action> m_axis_binding;

  std::map<Action, Command> m_action_binding;
  Status m_status;
};

#endif // PLAYER_H
