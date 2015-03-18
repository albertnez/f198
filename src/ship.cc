#include "ship.h"
#include "data_tables.h"
#include "utility.h"
#include "bullet.h"
#include "particle_node.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>


namespace {
std::vector<ShipData> Table = initialize_ship_data();
}

Ship::Ship(Type type, const ShaderHolder& shaders)
    : Entity(Table[type].hitpoints),
      m_type(type),
      m_shoot_power(static_cast<Ship::ShootPower>(Table[type].shoot_power)),
      m_time_since_shot(),
      m_fire_cooldown(Table[type].fire_cooldown),
      m_is_shooting(false),
      m_aim_dir(0.0f, 0.0f),
      m_shoot_dir(0.0f, 0.0f),
      m_spawning(true),
      m_particle_system(nullptr),
      m_shader(shaders.get(Table[type].shader)) {
  
  setPosition(Table[m_type].spawn_position);

  m_fire_command.action = [this] (SceneNode& scene_node, sf::Time /*dt*/) {
    create_bullet(scene_node);
  };
  m_fire_command.category = Category::ObjectLayer;
} 

unsigned int Ship::get_category() const {
  unsigned int category = Category::None;
  switch (m_type) {
    case Player: return Category::Player;
    case Chaser: category |= Category::Chaser;
    case Linear:
    case Static: category |= Category::Enemy;
    default: return category;
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
  return (get_category() & Category::Enemy && is_destroyed());
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

void Ship::upgrade_bullet(unsigned levels) {
  m_shoot_power = static_cast<ShootPower>(
      std::min(m_shoot_power + levels,
               static_cast<unsigned>(Table[m_type].max_shoot_power)));
}

void Ship::upgrade_fire_rate(sf::Time dif) {
  m_fire_cooldown = std::max(m_fire_cooldown - dif,
                             Table[m_type].min_fire_cooldown);
}

void Ship::update_current(sf::Time dt, CommandQueue& commands) {
  // Assign a particle emitter if it does not have any yet
  if (m_particle_system == nullptr) {
    auto finder = [this] (ParticleNode& container, sf::Time /*dt*/) {
      if (container.get_particle_type() == Particle::Explosion)
        m_particle_system = &container;
    };
		Command command;
		command.category = Category::ParticleSystem;
		command.action = derived_action<ParticleNode>(finder);
		commands.push(command);
  }

  // Update shoot time
  if (m_time_since_shot < m_fire_cooldown)
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

void Ship::on_destroy() {
  // Spawn all particles
  const int num_particles = 40;
  const float pi = atan(1)*4.0;
  const float step = (2.0f*pi)/num_particles;
  sf::Vector2f position = get_world_position();
  if (m_particle_system) {
    for (float angle = 0.0f; angle < (2.0f*pi); angle += step) {
      sf::Vector2f dir(std::sin(angle), std::cos(angle));
      m_particle_system->add_particle(position, dir);
    }
  }
}

void Ship::draw_current(sf::RenderTarget& target,
                              sf::RenderStates states) const {
  sf::RectangleShape rectangle(Table[m_type].size);
  rectangle.setFillColor(Table[m_type].color);
  center_origin(rectangle);

  sf::VertexArray quad(sf::Quads, 4);

  const sf::Vector2f half = Table[m_type].size * 0.5f;
  quad[0].position = sf::Vector2f(-half.x, -half.y);
  quad[1].position = sf::Vector2f(half.x, -half.y);
  quad[2].position = sf::Vector2f(half.x, half.y);
  quad[3].position = sf::Vector2f(-half.x, half.y);

  quad[0].texCoords = sf::Vector2f(0.0f, 0.0f);
  quad[1].texCoords = sf::Vector2f(1.0f, 0.0f);
  quad[2].texCoords = sf::Vector2f(1.0f, 1.0f);
  quad[3].texCoords = sf::Vector2f(0.0f, 1.0f);

  states.shader = &m_shader;

  target.draw(quad, states);
}

void Ship::try_shoot(sf::Time /*dt*/, CommandQueue& commands) {
  if (m_is_shooting && m_time_since_shot >= m_fire_cooldown &&
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
  if (get_category() & Category::Enemy) bullet_type = Bullet::Enemy;

  unsigned bullets = static_cast<unsigned>(m_shoot_power);
  sf::Vector2f offset_dir(-m_shoot_dir.y, m_shoot_dir.x);
  const float offset = 5.0f;
  const sf::Vector2f init_pos = get_world_position() - 
                                offset_dir*offset*((bullets-1)/2.0f);
  
  for (unsigned i = 0; i < bullets; ++i) {
    std::unique_ptr<Bullet> bullet(new Bullet(bullet_type));
    sf::Vector2f act_dir(m_shoot_dir);

    if (m_shoot_power == TwoTwoBullet || m_shoot_power == TwoThreeBullet) {
      if (i == 0 || i == bullets-1) {
        const float angle = (i ? 1 : -1) * 10.0f * (std::atan(1)/45.0f);
        act_dir = sf::Vector2f(
            m_shoot_dir.x * std::cos(angle) - m_shoot_dir.y * std::sin(angle),
            m_shoot_dir.x * std::sin(angle) + m_shoot_dir.y * std::cos(angle));
      }
    }
    sf::Vector2f velocity = unit_vector(act_dir)*bullet->get_max_speed();
    bullet->set_velocity(velocity);
    bullet->setPosition(init_pos + offset_dir*offset*float(i));
    scene_node.attach_child(std::move(bullet));
  }
}
