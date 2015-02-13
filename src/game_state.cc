#include "game_state.h"
#include <SFML/Graphics/RenderWindow.hpp>

GameState::GameState(StateStack& stack, Context context)
    : State(stack, context),
      m_world(*context.window, *context.fonts),
      m_player(*context.player) {
}

void GameState::draw() {
  m_world.draw();
}

bool GameState::update(sf::Time dt) {
  m_world.update(dt);

  CommandQueue& commands = m_world.get_command_queue();
  m_player.handle_realtime_input(commands);

  return true;
}

bool GameState::handle_event(const sf::Event& event) {
  // Game input handling
  CommandQueue& commands = m_world.get_command_queue();
  m_player.handle_event(event, commands);

  // Escape pressed, trigger the pause screen
  if (event.type == sf::Event::KeyPressed && 
      event.key.code == sf::Keyboard::Escape) {
    request_stack_pop();
  }

  return true;
}
