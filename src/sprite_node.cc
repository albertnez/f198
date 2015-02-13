#include "sprite_node.h"

#include <SFML/Graphics/RenderTarget.hpp>


SpriteNode::SpriteNode(const sf::Texture& texture) : m_sprite(texture) {
}  

SpriteNode::SpriteNode(const sf::Texture& texture,
                       const sf::IntRect& texture_rect)
    : m_sprite(texture, texture_rect) {
}

void SpriteNode::draw_current(sf::RenderTarget& target,
                              sf::RenderStates states) const {
  target.draw(m_sprite, states);
}
