#ifndef BULLET_H
#define BULLET_H

#include "entity.h"

class Bullet : public Entity {
public:
  enum Type {
    Ally,
    Enemy,
    TypeCount,
  };
  Bullet(Type type);
  unsigned int get_category() const override;
  // Override, bullets wont check against all scene
  void check_scene_collision(SceneNode& scene_graph, 
                             std::set<Pair>& collision_pairs) override;
  sf::FloatRect get_bounding_rect() const override;
  int get_damage() const;
  float get_max_speed() const;

private:
  void draw_current(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

  Type m_type;
};

#endif // BULLET_H
