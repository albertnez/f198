#include "world.h"
#include "bullet.h"
#include "utility.h"
#include "data_tables.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cmath>
#include <limits>

namespace {
std::vector<LevelData> Table = initialize_level_data();
}

World::World(sf::RenderTarget& outputTarget, FontHolder& fonts)
    : m_target(outputTarget),
      m_world_view(outputTarget.getDefaultView()),
      m_textures(),
      m_fonts(fonts),
      m_scene_graph(),
      m_scene_layers(),
      m_player(nullptr),
      m_command_queue(),
      m_size(800.0f, 600.0f),
      m_level(FirstLevel),
      m_time_since_spawn() {

  load_textures();
  build_scene();
}

void World::update(sf::Time dt) {
  // Update new enemies
  attempt_enemies_spawn(dt);

  // Update spawning status of enemies
  update_spawn_status(); 

  // Remove objects outside of bounds
  remove_outside_entities();

  // Forward commands to scene graph
  while (!m_command_queue.empty())
    m_scene_graph.on_command(m_command_queue.pop(), dt);

  // Guide enemies towards player
  guide_enemies();

  // Handle collisions
  handle_collisions();

  // Removed dead entities
  m_scene_graph.remove_wrecks();

  // Update scene graph
  m_scene_graph.update(dt, m_command_queue);
}

void World::draw() {
  m_target.setView(m_world_view);
  m_target.draw(m_scene_graph);
}

bool World::is_player_alive() const {
  return !m_player->is_destroyed();
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
    if (matches_categories(pair, Category::Enemy, Category::AllyBullet) ||
        matches_categories(pair, Category::Player, Category::EnemyBullet)) {
      auto& ship = static_cast<Ship&>(*pair.first);
      auto& bullet = static_cast<Bullet&>(*pair.second);

      ship.damage(bullet.get_damage());
      bullet.destroy();
    }
    else if (matches_categories(pair, Category::Enemy, Category::Player)) {
      auto& enemy = static_cast<Ship&>(*pair.first);
      auto& player = static_cast<Ship&>(*pair.second);

      player.damage(enemy.get_damage());
      enemy.destroy();
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
  m_player = player.get();
  m_scene_layers[ShipLayer]->attach_child(std::move(player));

  // Add one enemy
  for (int i = 0; i < 20; ++i) {
    std::unique_ptr<Ship> enemy(new Ship(Ship::Enemy));
    enemy->setPosition(20.0f + 40.0f*i, 20.0f);
    m_scene_layers[ShipLayer]->attach_child(std::move(enemy));
  }
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
    // don't remove if it is an enemy spawning
    Entity* pointer = &e;
    Ship* enemy = dynamic_cast<Ship*>(pointer);
    if (enemy && enemy->is_spawning())
      return;
		if (!get_bounding_rect().intersects(e.get_bounding_rect()))
			e.destroy();
	});

	m_command_queue.push(command);
}

void World::update_spawn_status() {
  Command command;
  command.category = Category::Enemy;
  command.action = derived_action<Ship>([this] (Ship& enemy, sf::Time) {
    if (enemy.is_spawning() && 
        get_bounding_rect().intersects(enemy.get_bounding_rect())) {
      enemy.unset_spawning();
    }
  });

  m_command_queue.push(command);
}

void World::guide_enemies() {
  Command guide_command;
  guide_command.category = Category::Enemy;
  guide_command.action = derived_action<Ship>([this] (Ship& enemy, sf::Time) {

    sf::Vector2f dir = unit_vector(m_player->get_world_position() - 
                                   enemy.get_world_position());
    sf::Vector2f velocity = dir * enemy.get_max_speed();

    enemy.set_velocity(velocity);
    enemy.aim(dir);
  });
  m_command_queue.push(guide_command);
}

void World::attempt_enemies_spawn(sf::Time dt) {
  m_time_since_spawn += dt;
  if (m_time_since_spawn > Table[m_level].spawn_cooldown) {
    m_time_since_spawn -= Table[m_level].spawn_cooldown;
    // spawn enemies
    for (unsigned i = 0; i < Table[m_level].num_enemies; ++i) {
      std::unique_ptr<Ship> enemy(new Ship(Ship::Enemy));
      enemy->setPosition(20.0f + 60.0f*i, -20.0f);
      m_scene_layers[ShipLayer]->attach_child(std::move(enemy));
    }
  }
}
