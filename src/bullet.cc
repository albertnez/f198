#include "bullet.h"
#include "data_tables.h"
#include "utility.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <iostream>

namespace {
std::vector<BulletData> Table = initialize_bullet_data();
}

Bullet::Bullet(Type type) :
  Entity(1),
  m_type(type) {
}

unsigned int Bullet::get_category() const {
  switch (m_type) {
    case Ally: return Category::AllyBullet;
    case Enemy: return Category::EnemyBullet;
    default: return Category::None;
  }
}

float Bullet::get_max_speed() const {
  return Table[m_type].speed;
}

void Bullet::draw_current(sf::RenderTarget& target,
                          sf::RenderStates states) const {
  sf::RectangleShape rectangle(Table[m_type].size);
  rectangle.setFillColor(Table[m_type].color);
  center_origin(rectangle);

  target.draw(rectangle, states);
}
