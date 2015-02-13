#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "state.h"
#include "world.h"
#include "player.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>


class GameState : public State {
public:
  GameState(StateStack& stack, Context context);

  virtual void draw() override;
  virtual bool update(sf::Time dt) override;
  virtual bool handle_event(const sf::Event& event) override;

private:
  World m_world;
  Player& m_player;
};

#endif // GAME_STATE_H
