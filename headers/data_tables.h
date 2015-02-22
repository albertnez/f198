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
  int damage;
  int score;
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


struct LevelData {
  // Formations consisting of positions of enemies in that formation
  std::vector<std::vector<sf::Vector2f>> formations;

  // Consists of an index of the formation, and waiting time from last round
  struct Round {
    unsigned formation;
    sf::Time wait_time;
    Round() : formation(0), wait_time() {}
    Round(unsigned formation, sf::Time wait_time)
        : formation(formation), wait_time(wait_time) {
    }
  };
  // All rounds that from current level
  std::vector<Round> rounds;
};

std::vector<ShipData> initialize_ship_data();
std::vector<BulletData> initialize_bullet_data();
std::vector<LevelData> initialize_level_data();

#endif // DATA_TABLES_H
