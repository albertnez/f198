#include "data_tables.h"
#include "ship.h"
#include "bullet.h"
#include "world.h"
#include "powerup.h"
#include "utility.h"

std::vector<ShipData> initialize_ship_data() {
  std::vector<ShipData> data(Ship::TypeCount);

  // PLAYER
  data[Ship::Player].hitpoints = 10;
  data[Ship::Player].speed = 600.0f;
  data[Ship::Player].fire_cooldown = sf::seconds(1.0f);
  data[Ship::Player].min_fire_cooldown = sf::seconds(0.1f);
  data[Ship::Player].shoot_power = Ship::SingleBullet;
  data[Ship::Player].max_shoot_power = Ship::QuintBullet;
  data[Ship::Player].size = sf::Vector2f(20.0f, 20.0f);
  data[Ship::Player].color = sf::Color::White;

  // CHASER
  data[Ship::Chaser].hitpoints = 2;
  data[Ship::Chaser].damage = 4;
  data[Ship::Chaser].score = 10;
  data[Ship::Chaser].speed = 80.0f;
  data[Ship::Chaser].fire_cooldown = sf::seconds(2.0f);
  data[Ship::Chaser].shoot_power = Ship::SingleBullet;
  data[Ship::Chaser].size = sf::Vector2f(20.0f, 20.0f);
  data[Ship::Chaser].color = sf::Color::Red;

  // LINEAR
  data[Ship::Linear].hitpoints = 2;
  data[Ship::Linear].damage = 4;
  data[Ship::Linear].score = 10;
  data[Ship::Linear].speed = 160.0f;
  data[Ship::Linear].fire_cooldown = sf::seconds(2.0f);
  data[Ship::Linear].shoot_power = Ship::SingleBullet;
  data[Ship::Linear].size = sf::Vector2f(20.0f, 20.0f);
  data[Ship::Linear].color = sf::Color::Red;
  

  // STATIC

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
      {
          {-20.0f, screen_height/2.0f},
          {screen_width/2.0f, screen_height+20.0f},
      },
  };
  data[World::FirstLevel].rounds = {
      {0, Ship::Chaser, sf::seconds(1.0f)},

      {0, Ship::Chaser, sf::seconds(5.0f)},

      {0, Ship::Chaser, sf::seconds(5.0f)},
      {1, Ship::Linear, sf::seconds(0.0f)},
  };
  data[World::FirstLevel].powerup_prob = 100;

  // LEVEL TWO
  data[World::SecondLevel].formations = {
      {
          {-20.0f, 250.0f},
          {-20.0f, 350.0f},
          {820.0f, 250.0f},
          {820.0f, 350.0f},
          {400.0f, -20.0f},
      },
      { // From Diagonals
          {-20.0f, -20.0f},
          {screen_width + 20.0f, -20.0f},
          {screen_width + 20.0f, screen_height + 20.0f},
          {-20.0f, screen_height + 20.0f},
      },
      { // From sides
          {screen_width/2.0f, -20.0f},
          {screen_width/2.0f, screen_height + 20.0f},
          {-20.0f, screen_height/2.0f},
          {screen_width + 20.0f, screen_height/2.0f},
      }
  };
  data[World::SecondLevel].rounds = {
      {0, Ship::Chaser, sf::seconds(3.0f)},
      {1, Ship::Linear, sf::seconds(0.0f)},

      {1, Ship::Chaser, sf::seconds(5.0f)},
      {2, Ship::Linear, sf::seconds(1.0f)},

      {1, Ship::Chaser, sf::seconds(5.0f)},
      {0, Ship::Chaser, sf::seconds(5.0f)},
  };
  data[World::SecondLevel].powerup_prob = 30;


  return data;
}

std::vector<PowerupData> initialize_powerup_data() {
  std::vector<PowerupData> data(Powerup::TypeCount);
  
  sf::Vector2f size(15.0f, 15.0f);

  data[Powerup::Heal].size = size;
  data[Powerup::Heal].color = sf::Color::Green;
  data[Powerup::Heal].action = [] (Ship& ship) { ship.heal(5); };

  data[Powerup::Bullet].size = size;
  data[Powerup::Bullet].color = sf::Color::Blue;
  data[Powerup::Bullet].action = std::bind(&Ship::upgrade_bullet, 
                                           std::placeholders::_1,
                                           1);

  data[Powerup::FireRate].size = size;
  data[Powerup::FireRate].color = sf::Color::Yellow;
  data[Powerup::FireRate].action = std::bind(&Ship::upgrade_fire_rate, 
                                             std::placeholders::_1,
                                             sf::seconds(0.2f));

  return data;
}
