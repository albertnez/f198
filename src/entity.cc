#include "entity.h"

#include <cassert>


Entity::Entity(int hitpoints) : m_velocity() , m_hitpoints(hitpoints) {
}

void Entity::set_velocity(sf::Vector2f velocity) {
  m_velocity = velocity;
}

void Entity::set_velocity(float vx, float vy) {
  m_velocity.x = vx;
  m_velocity.y = vy;
}

sf::Vector2f Entity::get_velocity() const {
  return m_velocity;
}

void Entity::accelerate(sf::Vector2f velocity) {
  m_velocity += velocity;
}

void Entity::accelerate(float vx, float vy) {
  m_velocity.x += vx;
  m_velocity.y += vy;
}

int Entity::get_hitpoints() const {
  return m_hitpoints;
}

void Entity::heal(int points) {
  assert(points > 0);

  m_hitpoints += points;
}

void Entity::damage(int points) {
  assert(points > 0);

  m_hitpoints -= points;
}

void Entity::destroy() {
  m_hitpoints = 0;
}

void Entity::remove() {
  destroy();
}

bool Entity::is_destroyed() const {
  return m_hitpoints <= 0;
}

void Entity::update_current(sf::Time dt, CommandQueue&) {  
  move(m_velocity * dt.asSeconds());
}
