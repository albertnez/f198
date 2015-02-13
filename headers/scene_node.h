#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include "category.h"

#include <SFML/System/NonCopyable.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include <vector>
#include <set>
#include <memory>
#include <utility>


struct Command;
class CommandQueue;

class SceneNode : public sf::Transformable, public sf::Drawable, 
                  private sf::NonCopyable {
public:
  typedef std::unique_ptr<SceneNode> Ptr;
  typedef std::pair<SceneNode*, SceneNode*> Pair;

public:
  explicit SceneNode(Category::Type category = Category::None);

  void attach_child(Ptr child);
  Ptr detach_child(const SceneNode& node);
  
  void update(sf::Time dt, CommandQueue& commands);

  sf::Vector2f get_world_position() const;
  sf::Transform get_world_transform() const;

  void on_command(const Command& command, sf::Time dt);
  virtual unsigned int get_category() const;

  void check_scene_collision(SceneNode& scene_graph, std::set<Pair>& collision_pairs);
  void check_node_collision(SceneNode& node, std::set<Pair>& collision_pairs);
  void remove_wrecks();
  virtual sf::FloatRect get_bounding_rect() const;
  virtual bool is_marked_for_removal() const;
  virtual bool is_destroyed() const;

private:
  virtual void update_current(sf::Time dt, CommandQueue& commands);
  void update_children(sf::Time dt, CommandQueue& commands);

  virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
  virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states) const;
  void draw_children(sf::RenderTarget& target, sf::RenderStates states) const;
  void draw_bounding_rect(sf::RenderTarget& target, sf::RenderStates states) const;

private:
  std::vector<Ptr> m_children;
  SceneNode* m_parent;
  Category::Type m_default_category;
};

bool collision(const SceneNode& lhs, const SceneNode& rhs);
float distance(const SceneNode& lhs, const SceneNode& rhs);

#endif // SCENE_NODE_H
