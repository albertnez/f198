#include "particle.h"

Particle::Particle(sf::Vector2f position, sf::Color color, sf::Time lifetime)
    : position(position),
      color(color),
      lifetime(lifetime) {
}

void Particle::update(sf::Time dt) {
  lifetime -= dt;
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
