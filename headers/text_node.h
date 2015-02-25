#ifndef TEXT_NODE_H
#define TEXT_NODE_H

#include "scene_node.h"
#include "resource_holder.h"
#include "resource_identifiers.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>


class TextNode : public SceneNode {
public:
  explicit TextNode(const FontHolder& font, 
                    const std::string& text, 
                    unsigned size = 20,
                    bool centered = false);

  virtual void draw_current(sf::RenderTarget& target, 
                            sf::RenderStates states) const override;

  void set_text(const std::string& text);

private:
  sf::Text m_text;
  bool m_centered;
};

#endif // TEXT_NODE_H
