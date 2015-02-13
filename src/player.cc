#include "player.h"
#include "command_queue.h"

#include <map>
#include <string>
#include <algorithm>


Player::Player() {
  // Set initial key bindings
  m_key_binding[sf::Keyboard::Left] = MoveLeft;
  m_key_binding[sf::Keyboard::Right] = MoveRight;
  m_key_binding[sf::Keyboard::Up] = MoveUp;
  m_key_binding[sf::Keyboard::Down] = MoveDown;
 
  // Set initial action bindings
  initialize_actions();  

  // Assign all categories to player
  for (auto& pair : m_action_binding)
    pair.second.category = Category::Player;
}

void Player::handle_event(const sf::Event& event, CommandQueue& commands) {
  if (event.type == sf::Event::KeyPressed) {
    // Check if pressed key appears in key binding, trigger command if so
    auto found = m_key_binding.find(event.key.code);
    if (found != m_key_binding.end() && !is_realtime_action(found->second))
      commands.push(m_action_binding[found->second]);
  }
}

void Player::handle_realtime_input(CommandQueue& commands) {
  // Traverse all assigned keys and check if they are pressed
  for (auto pair : m_key_binding) {
    // If key is pressed, lookup action and trigger corresponding command
    if (sf::Keyboard::isKeyPressed(pair.first) && is_realtime_action(pair.second))
      commands.push(m_action_binding[pair.second]);
  }
}

void Player::assign_key(Action action, sf::Keyboard::Key key) {
  // Remove all keys that already map to action
  for (auto itr = m_key_binding.begin(); itr != m_key_binding.end(); ) {
    if (itr->second == action) {
      m_key_binding.erase(itr++);
    }
    else {
      ++itr;
    }
  }
  // Insert new binding
  m_key_binding[key] = action;
}

sf::Keyboard::Key Player::get_assigned_key(Action action) const {
  for (auto pair : m_key_binding) {
    if (pair.second == action)
      return pair.first;
  }

  return sf::Keyboard::Unknown;
}

void Player::initialize_actions() {
  // for example
  //m_action_binding[MoveLeft].action = derived_action<PlayerClass>(
  //  [] (PlayerClass& player, sf::Time) {
  //    player.move_left(); 
  //  });
}

bool Player::is_realtime_action(Action action) {
  // Which actions are realtime
  switch (action) {
    case MoveLeft:
    case MoveRight:
    case MoveDown:
    case MoveUp:
      return true;
    default:
      return false;
  }
}
