#include "text_node.h"
#include "utility.h"

#include <SFML/Graphics/RenderTarget.hpp>


TextNode::TextNode(const FontHolder& fonts, 
                   const std::string& text, 
                   unsigned size)
    : m_text(text, fonts.get(Fonts::Main), size) {
  center_origin(m_text);
}  


void TextNode::draw_current(sf::RenderTarget& target,
                              sf::RenderStates states) const {
  target.draw(m_text, states);
}

void TextNode::set_text(const std::string& text) {
  m_text.setString(text);
  center_origin(m_text);
}
