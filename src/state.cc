#include "state.h"
#include "state_stack.h"


State::Context::Context(sf::RenderWindow& window, 
                        TextureHolder& textures,
                        FontHolder& fonts, 
                        Player& player)
    : window(&window),
      textures(&textures),
      fonts(&fonts),
      player(&player) {
}

State::State(StateStack& stack, Context context)
    : m_stack(&stack),
      m_context(context) {
}

State::~State() {
}

void State::request_stack_push(States::ID state_id) {
  m_stack->push(state_id);
}

void State::request_stack_pop() {
  m_stack->pop();
}

void State::request_state_clear() {
  m_stack->clear();
}

State::Context State::context() const {
  return m_context;
}
