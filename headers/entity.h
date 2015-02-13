#ifndef ENTITY_H
#define ENTITY_H

#include "scene_node.h"


class Entity : public SceneNode {
public:
  explicit Entity(int hitpoints);

  void set_velocity(sf::Vector2f velocity);
  void set_velocity(float vx, float vy);
  void accelerate(sf::Vector2f velocity);
  void accelerate(float vx, float vy);
  sf::Vector2f get_velocity() const;

  int get_hitpoints() const;
  void heal(int points);
  void damage(int points);
  void destroy();
  virtual void remove();
  virtual bool is_destroyed() const override;


protected:
  virtual void update_current(sf::Time dt, CommandQueue& commands) override;


private:
  sf::Vector2f m_velocity;
  int m_hitpoints;
};

#endif // ENTITY_H
