#ifndef STATE_H
#define STATE_H

#include "state_identifiers.h"
#include "resource_identifiers.h"

#include <SFML/System/Time.hpp>
#include <SFML/Window/Event.hpp>

#include <memory>


namespace sf {
class RenderWindow;
}

class StateStack;
class Player;

class State {
 public:
  typedef std::unique_ptr<State> Ptr;

  struct Context {
    Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts, Player& player);

    sf::RenderWindow* window;
    TextureHolder* textures;
    FontHolder* fonts;
    Player* player;
  };

  State(StateStack& stack, Context context);
  virtual ~State();

  virtual void draw() = 0;
  virtual bool update(sf::Time dt) = 0;
  virtual bool handle_event(const sf::Event& event) = 0;

 protected:
  void request_stack_push(States::ID state_id);
  void request_stack_pop();
  void request_state_clear();

  Context context() const;

 private:
  StateStack* m_stack;
  Context m_context;
};

#endif // STATE_H
