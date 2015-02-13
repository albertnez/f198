#include "scene_node.h"
#include "command.h"
#include "utility.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>


SceneNode::SceneNode(Category::Type category)
    : m_children()
    , m_parent(nullptr)
    , m_default_category(category) {
}

void SceneNode::attach_child(Ptr child) {
  child->m_parent = this;
  m_children.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detach_child(const SceneNode& node) {
  auto found = std::find_if(m_children.begin(), m_children.end(), [&] (Ptr& p) {
                              return p.get() == &node;
                            });
  assert(found != m_children.end());

  Ptr result = std::move(*found);
  result->m_parent = nullptr;
  m_children.erase(found);
  return result;
}

void SceneNode::update(sf::Time dt, CommandQueue& commands) {
  update_current(dt, commands);
  update_children(dt, commands);
}

void SceneNode::update_current(sf::Time, CommandQueue&) {
  // Do nothing by default
}

void SceneNode::update_children(sf::Time dt, CommandQueue& commands) {
  for (Ptr& child : m_children)
    child->update(dt, commands);
}

void SceneNode::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // Apply transform of current node
  states.transform *= getTransform();

  // Draw node and children with changed transform
  draw_current(target, states);
  draw_children(target, states);

  // Draw bounding rectangle - disabled by default
  //draw_bounding_rect(target, states);
}

void SceneNode::draw_current(sf::RenderTarget&, sf::RenderStates) const {
  // Do nothing by default
}

void SceneNode::draw_children(sf::RenderTarget& target, sf::RenderStates states) const {
  for (const Ptr& child : m_children)
    child->draw(target, states);
}

void SceneNode::draw_bounding_rect(sf::RenderTarget& target, sf::RenderStates) const {
  sf::FloatRect rect = get_bounding_rect();

  sf::RectangleShape shape;
  shape.setPosition(sf::Vector2f(rect.left, rect.top));
  shape.setSize(sf::Vector2f(rect.width, rect.height));
  shape.setFillColor(sf::Color::Transparent);
  shape.setOutlineColor(sf::Color::Green);
  shape.setOutlineThickness(1.f);

  target.draw(shape);
}

sf::Vector2f SceneNode::get_world_position() const {
  return get_world_transform() * sf::Vector2f();
}

sf::Transform SceneNode::get_world_transform() const {
  sf::Transform transform = sf::Transform::Identity;

  for (const SceneNode* node = this; node != nullptr; node = node->m_parent)
    transform = node->getTransform() * transform;

  return transform;
}

void SceneNode::on_command(const Command& command, sf::Time dt) {
  // Command current node, if category matches
  if (command.category & get_category())
    command.action(*this, dt);

  // Command children
  for (Ptr& child : m_children)
    child->on_command(command, dt);
}

unsigned int SceneNode::get_category() const {
  return m_default_category;
}

void SceneNode::check_scene_collision(SceneNode& scene_graph, 
                                      std::set<Pair>& collision_pairs) {

  check_node_collision(scene_graph, collision_pairs);

  for (Ptr& child : scene_graph.m_children)
    check_scene_collision(*child, collision_pairs);
}

void SceneNode::check_node_collision(SceneNode& node, 
                                     std::set<Pair>& collision_pairs) {
  if (this != &node && collision(*this, node) && 
      !is_destroyed() && !node.is_destroyed())
    collision_pairs.insert(std::minmax(this, &node));

  for (Ptr& child : m_children)
    child->check_node_collision(node, collision_pairs);
}

void SceneNode::remove_wrecks() {
  // Remove all children which request so
  auto wreckfield_begin = std::remove_if(
      m_children.begin(), 
      m_children.end(), 
      std::mem_fn(&SceneNode::is_marked_for_removal));
  m_children.erase(wreckfield_begin, m_children.end());

  // Call function recursively for all remaining children
  std::for_each(m_children.begin(), 
                m_children.end(),
                std::mem_fn(&SceneNode::remove_wrecks));
}

sf::FloatRect SceneNode::get_bounding_rect() const {
  return sf::FloatRect();
}

bool SceneNode::is_marked_for_removal() const {
  // By default, remove node if entity is destroyed
  return is_destroyed();
}

bool SceneNode::is_destroyed() const {
  // By default, scene node needn't be removed
  return false;
}

bool collision(const SceneNode& lhs, const SceneNode& rhs) {
  return lhs.get_bounding_rect().intersects(rhs.get_bounding_rect());
}

float distance(const SceneNode& lhs, const SceneNode& rhs) {
  return length(lhs.get_world_position() - rhs.get_world_position());
}
