#ifndef SPRITE_NODE_H
#define SPRITE_NODE_H

#include "scene_node.h"

#include <SFML/Graphics/Sprite.hpp>


class SpriteNode : public SceneNode {
public:
  explicit SpriteNode(const sf::Texture& texture);
  SpriteNode(const sf::Texture& texture, const sf::IntRect& textureRect);

  virtual void draw_current(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  sf::Sprite m_sprite;
};

#endif // SPRITE_NODE_H
