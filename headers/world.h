#ifndef WORLD_H
#define WORLD_H

#include "resource_holder.h"
#include "resource_identifiers.h"
#include "scene_node.h"
#include "sprite_node.h"
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
  explicit World(sf::RenderTarget& outputTarget, FontHolder& fonts);
  void update(sf::Time dt);
  void draw();
  
  CommandQueue& get_command_queue();

private:
  void load_textures();
  void handle_collisions();
  void build_scene();
  sf::FloatRect get_view_bounds() const;

  // Layers enum
  enum Layer {
    LayerCount,
  };

  sf::RenderTarget& m_target;
  sf::View m_world_view;
  TextureHolder m_textures;
  FontHolder& m_fonts;

  SceneNode m_scene_graph;
  std::array<SceneNode*, LayerCount>  m_scene_layers;
  CommandQueue m_command_queue;
};

#endif // WORLD_H
