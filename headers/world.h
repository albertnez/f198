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
  void remove_outside_entities();
  void update_spawn_status();
  void guide_enemies();
  void attempt_enemies_spawn(sf::Time dt);

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
  Level m_level;
  sf::Time m_time_since_spawn;
};

#endif // WORLD_H
