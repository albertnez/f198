#ifndef WORLD_H
#define WORLD_H

#include "resource_holder.h"
#include "resource_identifiers.h"
#include "scene_node.h"
#include "sprite_node.h"
#include "ship.h"
#include "command_queue.h"
#include "command.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <array>
#include <queue>


// Forward declaration
namespace sf {
class RenderTarget;
}

class World : private sf::NonCopyable {
public:
  enum Level {
    FirstLevel,
    SecondLevel,
    LevelCount,
  };

  explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts);
  void update(sf::Time dt);
  void draw();
  bool is_player_alive() const;
  
  CommandQueue& get_command_queue();

private:
  void load_textures();
  void handle_collisions();
  void build_scene();
  sf::FloatRect get_view_bounds() const;
  sf::FloatRect get_bounding_rect() const;
  // Remove entities that are out of bounds, but not those that are spawning
  void remove_outside_entities();
  // If spawning enemies reached the game bounding rect, update status
  void update_spawn_status();
  // Guides enemies to player
  void guide_enemies();
  // Spawns enemies if possible, and updates current level and round
  void update_level_status(sf::Time dt);
  // Adjust player position so that doesn't go out of bounds
  void adjust_player_position();

  // Layers enum
  enum Layer {
    ObjectLayer,
    ShipLayer,
    LayerCount,
  };

  sf::RenderTarget& m_target;
  sf::View m_world_view;
  TextureHolder m_textures;
  FontHolder& m_fonts;

  SceneNode m_scene_graph;
  std::array<SceneNode*, LayerCount>  m_scene_layers;
  Ship* m_player;
  CommandQueue m_command_queue;
  sf::Vector2f m_size;
  unsigned m_level;
  unsigned m_level_round;
  sf::Time m_time_since_spawn;
  // number of alive enemies, counting spawning ones
  unsigned m_alive_enemies;
};

#endif // WORLD_H
