#ifndef SHIP_H
#define SHIP_H

#include "entity.h"
#include "command.h"
#include "command_queue.h"

class Ship : public Entity {
public:
  enum Type {
    Player,
    Chaser, // Follows the player
    Linear, // Doesn't change direction
    Static, // Is static
    TypeCount,
  };
  enum ShootPower {
    ZeroBullet,
    SingleBullet,
    DoubleBullet,
    TripleBullet,
    QuadBullet,
    QuintBullet,
    ShootPowerCount,
  };
  explicit Ship(Type type);
  unsigned int get_category() const override;
  sf::FloatRect get_bounding_rect() const override;
  bool is_marked_for_removal() const override;
  int get_damage() const;
  int get_score() const;
  float get_max_speed() const;
  void aim(sf::Vector2f dir);
  bool is_spawning() const;
  void unset_spawning();
  void upgrade_bullet(unsigned levels);
  void upgrade_fire_rate(sf::Time dif);

private:
  void update_current(sf::Time dt, CommandQueue& commands) override;
  void draw_current(sf::RenderTarget& target, 
                    sf::RenderStates states) const override;
  void try_shoot(sf::Time dt, CommandQueue& commands);
  void create_bullet(SceneNode& scene_node) const;

  Type m_type;
  ShootPower m_shoot_power;
  Command m_fire_command;
  sf::Time m_time_since_shot;
  sf::Time m_fire_cooldown;
  bool m_is_shooting;
  sf::Vector2f m_aim_dir;
  sf::Vector2f m_shoot_dir;
  bool m_spawning;
};

#endif // SHIP_H
