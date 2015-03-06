#include "particle.h"

Particle::Particle(sf::Vector2f position, sf::Color color, sf::Time lifetime,
                   sf::Vector2f speed)
    : position(position),
      color(color),
      lifetime(lifetime),
      speed(speed) {
}

void Particle::update(sf::Time dt) {
  lifetime -= dt;
  position += speed * dt.asSeconds();
}

sf::Vector2f Particle::get_position() const {
  return position;
}

sf::Color Particle::get_color() const {
  return color;
}

sf::Time Particle::get_lifetime() const {
  return lifetime;
}
