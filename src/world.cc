#include "world.h"
#include "ship.h"
#include "bullet.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

World::World(sf::RenderTarget& outputTarget, FontHolder& fonts)
    : m_target(outputTarget),
      m_world_view(outputTarget.getDefaultView()),
      m_textures(),
      m_fonts(fonts),
      m_scene_graph(),
      m_scene_layers(),
      m_size(800.0f, 600.0f) {

  load_textures();
  build_scene();
}

void World::update(sf::Time dt) {
  // Forward commands to scene graph
  while (!m_command_queue.empty())
    m_scene_graph.on_command(m_command_queue.pop(), dt);

  // Handle collisions
  handle_collisions();

  // Remove objects outside of bounds
  remove_outside_entities();

  // Removed dead entities
  m_scene_graph.remove_wrecks();

  // Update scene graph
  m_scene_graph.update(dt, m_command_queue);
}

void World::draw() {
  m_target.setView(m_world_view);
  m_target.draw(m_scene_graph);
}

CommandQueue& World::get_command_queue() {
  return m_command_queue;
}

void World::load_textures() {
  //m_textures.load(Textures::ID, "path/to/file.png");
}

bool matches_categories(SceneNode::Pair& colliders, 
                        Category::Type type1, 
                        Category::Type type2) {
  unsigned int category1 = colliders.first->get_category();
  unsigned int category2 = colliders.second->get_category();

  // Make sure first pair entry has category type1 and second has type2
  if (type1 & category1 && type2 & category2) {
    return true;
  }
  else if (type1 & category2 && type2 & category1) {
    std::swap(colliders.first, colliders.second);
    return true;
  }
  else {
    return false;
  }
}

void World::handle_collisions() {
  std::set<SceneNode::Pair> collision_pairs;
  m_scene_graph.check_scene_collision(m_scene_graph, collision_pairs);

  for (SceneNode::Pair pair : collision_pairs) {
    if (matches_categories(pair, Category::Enemy, Category::AllyBullet)) {
      auto& enemy = static_cast<Ship&>(*pair.first);
      auto& bullet = static_cast<Bullet&>(*pair.second);

      enemy.damage(bullet.get_damage());
      bullet.destroy();
    }
  }
}

void World::build_scene() {
  // Initialize the different layers
  for (std::size_t i = 0; i < LayerCount; ++i) {
    Category::Type category = Category::None;
    if (i == Layer::ObjectLayer) category = Category::ObjectLayer;
    SceneNode::Ptr layer(new SceneNode(category));
    m_scene_layers[i] = layer.get();
    m_scene_graph.attach_child(std::move(layer));
  }
  // Initialize remaining scene
	std::unique_ptr<Ship> player(new Ship(Ship::Player));
  m_scene_layers[ShipLayer]->attach_child(std::move(player));

  // Add one enemy
  std::unique_ptr<Ship> enemy(new Ship(Ship::Enemy));
  enemy->setPosition(20.0f, 20.0f);
  m_scene_layers[ShipLayer]->attach_child(std::move(enemy));
}

sf::FloatRect World::get_view_bounds() const {
  return sf::FloatRect(m_world_view.getCenter() - 
         m_world_view.getSize() / 2.f, m_world_view.getSize());
}

sf::FloatRect World::get_bounding_rect() const {
  return sf::FloatRect(0.0f, 0.0f, m_size.x, m_size.y);
}

void World::remove_outside_entities() {
  Command command;
	command.category = Category::AllyBullet | Category::EnemyBullet | 
                     Category::Enemy;
	command.action = derived_action<Entity>([this] (Entity& e, sf::Time) {
		if (!get_bounding_rect().intersects(e.get_bounding_rect()))
			e.destroy();
	});

	m_command_queue.push(command);
}
