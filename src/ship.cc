#include "ship.h"
#include "data_tables.h"
#include "utility.h"
#include "bullet.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


namespace {
std::vector<ShipData> Table = initialize_ship_data();
}

Ship::Ship(Type type)
    : Entity(Table[type].hitpoints),
      m_type(type),
      m_time_since_shot(),
      m_is_shooting(false),
      m_aim_dir(0.0f, 0.0f),
      m_shoot_dir(0.0f, 0.0f),
      m_spawning(true) {
  setPosition(Table[m_type].spawn_position);

  m_fire_command.action = [this] (SceneNode& scene_node, sf::Time dt) {
    create_bullet(scene_node);
  };
  m_fire_command.category = Category::ObjectLayer;
} 

unsigned int Ship::get_category() const {
  switch (m_type) {
    case Player: return Category::Player;
    case Enemy: return Category::Enemy;
    default: return Category::None;
  }
}

sf::FloatRect Ship::get_bounding_rect() const {
  // If destroyed, no collisions
  if (is_destroyed()) return sf::FloatRect();
  sf::Vector2f pos = sf::Vector2f(0.0f, 0.0f);
  sf::Vector2f size = Table[m_type].size;
  return get_world_transform().transformRect(sf::FloatRect(pos-size/2.f, size));
}

bool Ship::is_marked_for_removal() const {
  return (m_type == Enemy && is_destroyed());
}

int Ship::get_damage() const {
  return Table[m_type].damage;
}

int Ship::get_score() const {
  return Table[m_type].score;
}

float Ship::get_max_speed() const {
  return Table[m_type].speed;
}

void Ship::aim(sf::Vector2f dir) {
  m_aim_dir += dir;
  m_is_shooting = true;
}

bool Ship::is_spawning() const {
  return m_spawning;
}

void Ship::unset_spawning() {
  m_spawning = false;
}

void Ship::update_current(sf::Time dt, CommandQueue& commands) {
  // Update shoot time
  if (m_time_since_shot < Table[m_type].fire_cooldown)
    m_time_since_shot += dt;
  // Limit speed
  if (length(get_velocity()) > get_max_speed()) {
    sf::Vector2f dir = unit_vector(get_velocity());
    set_velocity(dir * get_max_speed());
  }

  // Check if we can fire a bullet
  try_shoot(dt, commands);

  Entity::update_current(dt, commands);

  // Reset velocity
  if (get_category() == Category::Player) {
    set_velocity(sf::Vector2f());
  }
}

void Ship::draw_current(sf::RenderTarget& target,
                              sf::RenderStates states) const {
  sf::RectangleShape rectangle(Table[m_type].size);
  rectangle.setFillColor(Table[m_type].color);
  center_origin(rectangle);

  target.draw(rectangle, states);
}

void Ship::try_shoot(sf::Time dt, CommandQueue& commands) {
  if (m_is_shooting && m_time_since_shot >= Table[m_type].fire_cooldown &&
      m_aim_dir != sf::Vector2f(0.0f, 0.0f)) {
    m_time_since_shot = sf::seconds(0.0f);
    commands.push(m_fire_command);
  }
  // Reset shooting values
  m_shoot_dir = m_aim_dir;
  m_aim_dir = sf::Vector2f(0.0f, 0.0f);
  m_is_shooting = false;
}

void Ship::create_bullet(SceneNode& scene_node) const {
  Bullet::Type bullet_type = Bullet::Ally;
  if (m_type == Enemy) bullet_type = Bullet::Enemy;

  std::unique_ptr<Bullet> bullet(new Bullet(bullet_type));
  // set speed
  sf::Vector2f velocity = unit_vector(m_shoot_dir)*bullet->get_max_speed();
  bullet->set_velocity(velocity);
  bullet->setPosition(get_world_position());
  scene_node.attach_child(std::move(bullet));
}
