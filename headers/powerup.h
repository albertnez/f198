#ifndef POWERUP_H
#define POWERUP_H

#include "entity.h"

class Ship;

class Powerup : public Entity {
public:
  enum Type {
    Heal,
    Bullet,
    FireRate,
    TypeCount,
  };
  explicit Powerup(Type type);
  unsigned int get_category() const override;
  void check_scene_collision(SceneNode& scene_graph,
                             std::set<Pair>& collision_pairs) override;
  sf::FloatRect get_bounding_rect() const override;

  void apply(Ship& ship);

private:
  void draw_current(sf::RenderTarget& target,
                    sf::RenderStates states) const override;
  // ADD DRAW
  Type m_type;

};

#endif // POWERUP_H
