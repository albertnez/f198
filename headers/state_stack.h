#ifndef STATE_STACK_H
#define STATE_STACK_H

#include "state.h"
#include "state_identifiers.h"
#include "resource_identifiers.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>

#include <vector>
#include <utility>
#include <functional>
#include <map>


namespace sf {
class Event;
class RenderWindow;
}

class StateStack : private sf::NonCopyable {
public:
  enum Action {
    Push,
    Pop,
    Clear,
  };

public:    
  explicit StateStack(State::Context context);

  template <typename T>
  void register_state(States::ID state_id);

  void update(sf::Time dt);
  void draw();
  void handle_event(const sf::Event& event);

  void push(States::ID state_id);
  void pop();
  void clear();

  bool empty() const;

private:
  State::Ptr create_state(States::ID state_id);
  void apply_pending_changes();

  struct PendingChange {
    explicit PendingChange(Action action, States::ID state_id = States::None);

    Action action;
    States::ID state_id;
  };

  std::vector<State::Ptr> m_stack;
  std::vector<PendingChange> m_pending_list;

  State::Context m_context;
  std::map<States::ID, std::function<State::Ptr()>> m_factories;
};

template <typename T>
void StateStack::register_state(States::ID state_id) {
  m_factories[state_id] = [this] () {
    return State::Ptr(new T(*this, m_context));
  };
}

#endif // STATE_STACK_H
