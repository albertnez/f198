#include "data_tables.h"
#include "ship.h"
#include "bullet.h"
#include "world.h"
#include "powerup.h"
#include "utility.h"

std::vector<ShipData> initialize_ship_data() {
  std::vector<ShipData> data(Ship::TypeCount);

  data[Ship::Player].hitpoints = 10;
  data[Ship::Player].speed = 600.0f;
  data[Ship::Player].fire_cooldown = sf::seconds(0.1f);
  data[Ship::Player].min_fire_cooldown = sf::seconds(0.1f);
  data[Ship::Player].shoot_power = Ship::SingleBullet;
  data[Ship::Player].max_shoot_power = Ship::TripleBullet;
  data[Ship::Player].spawn_position = sf::Vector2f(200.0f, 200.0f);
  data[Ship::Player].size = sf::Vector2f(20.0f, 20.0f);
  data[Ship::Player].color = sf::Color::White;

  data[Ship::Enemy].hitpoints = 2;
  data[Ship::Enemy].damage = 100.0f;
  data[Ship::Enemy].score = 10;
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

  // LEVEL ONE
  data[World::FirstLevel].formations = {
      {
          {300.0f, -20.0f},
          {500.0f, -20.0f},
      },
  };
  data[World::FirstLevel].rounds = {
      {0, sf::seconds(1.0f)},
      {0, sf::seconds(5.0f)},
  };

  // LEVEL TWO
  data[World::SecondLevel].formations = {
      {
          {-20.0f, 250.0f},
          {-20.0f, 350.0f},
          {820.0f, 250.0f},
          {820.0f, 350.0f},
          {400.0f, -20.0f},
      },
      {
          {-20.0f, -20.0f},
          {screen_width + 20.0f, -20.0f},
          {screen_width + 20.0f, screen_height + 20.0f},
          {-20.0f, screen_height + 20.0f},
      },
  };
  data[World::SecondLevel].rounds = {
      {0, sf::seconds(3.0f)},
      {1, sf::seconds(5.0f)},
      {1, sf::seconds(5.0f)},
      {0, sf::seconds(5.0f)},
  };


  return data;
}

std::vector<PowerupData> initialize_powerup_data() {
  std::vector<PowerupData> data(Powerup::TypeCount);
  
  data[Powerup::Heal].action = [] (Ship& ship) { ship.heal(5); };

  data[Powerup::Bullet].action = std::bind(&Ship::upgrade_bullet, 
                                           std::placeholders::_1,
                                           1);

  data[Powerup::FireRate].action = std::bind(&Ship::upgrade_fire_rate, 
                                             std::placeholders::_1,
                                             sf::seconds(0.2f));
}
