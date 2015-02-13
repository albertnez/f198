#include "state_stack.h"

#include <cassert>


StateStack::StateStack(State::Context context)
    : m_stack(),
      m_pending_list(),
      m_context(context),
      m_factories() {
}

void StateStack::update(sf::Time dt) {
  // Iterate from top to bottom, stop as soon as update() returns false
  for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr) {
    if (!(*itr)->update(dt))
      break;
  }
  apply_pending_changes();
}

void StateStack::draw() {
  // Draw all active states from bottom to top
  for (State::Ptr& state : m_stack)
    state->draw();
}

void StateStack::handle_event(const sf::Event& event) {
  // Iterate from top to bottom, stop as soon as handle_event() returns false
  for (auto itr = m_stack.rbegin(); itr != m_stack.rend(); ++itr) {
    if (!(*itr)->handle_event(event))
      break;
  }
  apply_pending_changes();
}

void StateStack::push(States::ID state_id) {
  m_pending_list.push_back(PendingChange(Push, state_id));
}

void StateStack::pop() {
  m_pending_list.push_back(PendingChange(Pop));
}

void StateStack::clear() {
  m_pending_list.push_back(PendingChange(Clear));
}

bool StateStack::empty() const {
  return m_stack.empty();
}

State::Ptr StateStack::create_state(States::ID state_id) {
  auto found = m_factories.find(state_id);
  assert(found != m_factories.end());

  return found->second();
}

void StateStack::apply_pending_changes() {
  for (PendingChange change : m_pending_list) {
    switch (change.action) {
      case Push:
        m_stack.push_back(create_state(change.state_id));
        break;

      case Pop:
        m_stack.pop_back();
        break;

      case Clear:
        m_stack.clear();
        break;
    }
  }

  m_pending_list.clear();
}

StateStack::PendingChange::PendingChange(Action action, States::ID state_id)
    : action(action),
      state_id(state_id) {
}
