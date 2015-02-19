#include "data_tables.h"
#include "ship.h"
#include "bullet.h"
#include "world.h"

std::vector<ShipData> initialize_ship_data() {
  std::vector<ShipData> data(Ship::TypeCount);

  data[Ship::Player].hitpoints = 10;
  data[Ship::Player].speed = 600.0f;
  data[Ship::Player].acceleration = 200.0f;
  data[Ship::Player].friction = 0.90f;
  data[Ship::Player].fire_cooldown = sf::seconds(0.1f);
  data[Ship::Player].spawn_position = sf::Vector2f(200.0f, 200.0f);
  data[Ship::Player].size = sf::Vector2f(20.0f, 20.0f);
  data[Ship::Player].color = sf::Color::White;

  data[Ship::Enemy].hitpoints = 2;
  data[Ship::Enemy].damage = 100.0f;
  data[Ship::Enemy].speed = 80.0f;
  data[Ship::Enemy].fire_cooldown = sf::seconds(2.0f);
  data[Ship::Enemy].spawn_position = sf::Vector2f(800.0f, 200.0f);
  data[Ship::Enemy].size = sf::Vector2f(20.0f, 20.0f);
  data[Ship::Enemy].color = sf::Color::Red;

  return data;
}

std::vector<BulletData> initialize_bullet_data() {
  std::vector<BulletData> data(Bullet::TypeCount);

  int damage = 1;
  float speed = 700.0f;
  sf::Vector2f size(4.0f, 4.0f);

  data[Bullet::Ally].damage = damage;
  data[Bullet::Ally].speed = speed;
  data[Bullet::Ally].size = size;
  data[Bullet::Ally].color = sf::Color::White;

  data[Bullet::Enemy].damage = damage;
  data[Bullet::Enemy].speed = 300.0f;
  data[Bullet::Enemy].size = size;
  data[Bullet::Enemy].color = sf::Color::Red;

  return data;
}

std::vector<LevelData> initialize_level_data() {
  std::vector<LevelData> data(World::LevelCount);

  data[World::FirstLevel].spawn_cooldown = sf::seconds(1.0f);
  data[World::FirstLevel].num_enemies = 5;

  return data;
}
