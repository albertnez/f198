#include "text_node.h"
#include "utility.h"

#include <SFML/Graphics/RenderTarget.hpp>


TextNode::TextNode(const FontHolder& fonts, 
                   const std::string& text, 
                   unsigned size,
                   bool centered)
    : m_text(text, fonts.get(Fonts::Main), size), m_centered(centered) {
  if (centered)
    center_origin(m_text);
}  


void TextNode::draw_current(sf::RenderTarget& target,
                              sf::RenderStates states) const {
  target.draw(m_text, states);
}

void TextNode::set_text(const std::string& text) {
  m_text.setString(text);
  if (m_centered)
    center_origin(m_text);
}
