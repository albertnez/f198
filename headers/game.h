#ifndef GAME_H
#define GAME_H

#include "resource_holder.h"
#include "resource_identifiers.h"
#include "player.h"
#include "state_stack.h"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>


class Game {
public:
  Game();
  void run();
  
private:
  void process_input();
  void update(sf::Time dt);
  void render();

  void update_statistics(sf::Time dt);
  void register_states();

private:
  static const sf::Time time_per_frame;

  sf::RenderWindow m_window;
  TextureHolder m_textures;
  FontHolder m_fonts;
  Player m_player;

  StateStack m_state_stack;

  sf::Text m_statistics_text;
  sf::Time m_statistics_update_time;
  std::size_t m_statistics_num_frames;
};

#endif // GAME_H
