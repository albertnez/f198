#include "player.h"
#include "command_queue.h"
#include "ship.h"

#include <map>
#include <string>
#include <algorithm>

struct ShipMover {
  ShipMover(float x, float y) : velocity(x, y) {}

  void operator() (Ship& ship, sf::Time dt) const {
    ship.accelerate(velocity * ship.get_max_speed());
  }
  sf::Vector2f velocity;
};

Player::Player() 
    : m_key_binding(),
      m_action_binding(),
      m_status(Status::Alive) {
  // Set initial key bindings
  m_key_binding[sf::Keyboard::A] = MoveLeft;
  m_key_binding[sf::Keyboard::D] = MoveRight;
  m_key_binding[sf::Keyboard::W] = MoveUp;
  m_key_binding[sf::Keyboard::S] = MoveDown;
 
  m_key_binding[sf::Keyboard::J] = FireLeft;
  m_key_binding[sf::Keyboard::L] = FireRight;
  m_key_binding[sf::Keyboard::I] = FireUp;
  m_key_binding[sf::Keyboard::K] = FireDown;

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

Player::Status Player::get_status() const {
  return m_status;
}

void Player::set_status(Status status) {
  m_status = status;
}

void Player::initialize_actions() {
  m_action_binding[MoveLeft].action = derived_action<Ship>(ShipMover(-1.0f, 0.0f));
  m_action_binding[MoveRight].action = derived_action<Ship>(ShipMover(1.0f, 0.0f));
  m_action_binding[MoveUp].action = derived_action<Ship>(ShipMover(0.0f, -1.0f));
  m_action_binding[MoveDown].action = derived_action<Ship>(ShipMover(0.0f, 1.0f));

  m_action_binding[FireLeft].action = derived_action<Ship>([] (Ship& ship, sf::Time dt) {
    ship.aim(sf::Vector2f(-1.0f, 0.0f));
  });
  m_action_binding[FireRight].action = derived_action<Ship>([] (Ship& ship, sf::Time dt) {
    ship.aim(sf::Vector2f(1.0f, 0.0f));
  });
  m_action_binding[FireDown].action = derived_action<Ship>([] (Ship& ship, sf::Time dt) {
    ship.aim(sf::Vector2f(0.0f, 1.0f));
  });
  m_action_binding[FireUp].action = derived_action<Ship>([] (Ship& ship, sf::Time dt) {
    ship.aim(sf::Vector2f(0.0f, -1.0f));
  });
}

bool Player::is_realtime_action(Action action) {
  // Which actions are realtime
  switch (action) {
    case MoveLeft:
    case MoveRight:
    case MoveDown:
    case MoveUp:
    case FireLeft:
    case FireRight:
    case FireDown:
    case FireUp:
      return true;
    default:
      return false;
  }
}
