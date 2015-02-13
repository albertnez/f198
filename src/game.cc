#include "game.h"
#include "utility.h"
#include "state.h"
#include "state_identifiers.h"
#include "game_state.h"


const sf::Time Game::time_per_frame = sf::seconds(1.f/60.f);

Game::Game()
    : m_window(sf::VideoMode(1024, 768), "f198", sf::Style::Close),
      m_textures(),
      m_fonts(),
      m_player(),
      m_state_stack(State::Context(m_window, m_textures, m_fonts, m_player)),
      m_statistics_text(),
      m_statistics_update_time(),
      m_statistics_num_frames(0) {

  m_window.setKeyRepeatEnabled(false);
  m_window.setVerticalSyncEnabled(true);

  m_fonts.load(Fonts::Main, "res/fonts/sansation.ttf");

  m_statistics_text.setFont(m_fonts.get(Fonts::Main));
  m_statistics_text.setPosition(5.f, 5.f);
  m_statistics_text.setCharacterSize(10u);
  m_statistics_text.setString("FPS: 0");

  register_states();
  m_state_stack.push(States::Game);
}

void Game::run() {
  sf::Clock clock;
  sf::Time time_since_last_update = sf::Time::Zero;

  while (m_window.isOpen()) {
    sf::Time dt = clock.restart();
    time_since_last_update += dt;

    while (time_since_last_update > time_per_frame) {
      time_since_last_update -= time_per_frame;

      process_input();
      update(time_per_frame);

      // Check inside this loop, because stack might be empty before update() call
      if (m_state_stack.empty())
        m_window.close();
    }

    update_statistics(dt);
    render();
  }
}

void Game::process_input() {
  sf::Event event;
  while (m_window.pollEvent(event)) {
    m_state_stack.handle_event(event);

    if (event.type == sf::Event::Closed)
      m_window.close();
  }
}

void Game::update(sf::Time dt) {
  m_state_stack.update(dt);
}

void Game::render() {
  m_window.clear();

  m_state_stack.draw();

  m_window.setView(m_window.getDefaultView());
  m_window.draw(m_statistics_text);

  m_window.display();
}

void Game::update_statistics(sf::Time dt) {
  m_statistics_update_time += dt;
  m_statistics_num_frames += 1;
  if (m_statistics_update_time >= sf::seconds(1.0f)) {
    m_statistics_text.setString("FPS: " + 
                                std::to_string(m_statistics_num_frames));

    m_statistics_update_time -= sf::seconds(1.0f);
    m_statistics_num_frames = 0;
  }
}

void Game::register_states() {
  // Register all different states here with their ID
  m_state_stack.register_state<GameState>(States::Game);
}
