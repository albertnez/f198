#ifndef DATA_TABLES_H
#define DATA_TABLES_H

#include "resource_identifiers.h"

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>

#include <vector>

// Contains all static initial data of objects
struct ShipData {
  int hitpoints;
  float speed;
  float acceleration;
  float friction;
  sf::Time fire_cooldown;
  sf::Vector2f spawn_position;
  sf::Vector2f size;
  sf::Color color;
};

struct BulletData {
  int damage;
  float speed;
  sf::Vector2f size;
  sf::Color color;
};

std::vector<ShipData> initialize_ship_data();
std::vector<BulletData> initialize_bullet_data();

#endif // DATA_TABLES_H
