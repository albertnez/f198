#include "data_tables.h"
#include "ship.h"
#include "bullet.h"

std::vector<ShipData> initialize_ship_data() {
  std::vector<ShipData> data(Ship::TypeCount);

  data[Ship::Player].hitpoints = 10;
  data[Ship::Player].speed = 600.0f;
  data[Ship::Player].acceleration = 200.0f;
  data[Ship::Player].friction = 0.85f;
  data[Ship::Player].fire_cooldown = sf::seconds(0.1f);
  data[Ship::Player].spawn_position = sf::Vector2f(200.0f, 200.0f);
  data[Ship::Player].size = sf::Vector2f(20.0f, 20.0f);
  data[Ship::Player].color = sf::Color::White;

  data[Ship::Enemy].hitpoints = 2;
  data[Ship::Enemy].speed = 40.0f;
  data[Ship::Enemy].fire_cooldown = sf::seconds(0.5f);
  data[Ship::Enemy].spawn_position = sf::Vector2f(800.0f, 200.0f);
  data[Ship::Enemy].size = sf::Vector2f(20.0f, 20.0f);
  data[Ship::Enemy].color = sf::Color::Red;

  return data;
}

std::vector<BulletData> initialize_bullet_data() {
  std::vector<BulletData> data(Bullet::TypeCount);

  int damage = 1;
  float speed = 800.0f;
  sf::Vector2f size(3.0f, 3.0f);

  data[Bullet::Ally].damage = damage;
  data[Bullet::Ally].speed = speed;
  data[Bullet::Ally].size = size;
  data[Bullet::Ally].color = sf::Color::White;

  data[Bullet::Enemy].damage = damage;
  data[Bullet::Enemy].speed = speed;
  data[Bullet::Enemy].size = size;
  data[Bullet::Enemy].color = sf::Color::Red;

  return data;
}

