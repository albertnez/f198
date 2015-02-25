#include "powerup.h"
#include "ship.h"
#include "data_tables.h"
#include "utility.h"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

namespace {
std::vector<PowerupData> Table = initialize_powerup_data();
}

Powerup::Powerup(Type type)
    : Entity(1),
      m_type(type) {
}

void Powerup::apply(Ship& ship) {
  Table[m_type].action(ship);
}

void Powerup::draw_current(sf::RenderTarget& target,
                           sf::RenderStates states) const {
  sf::RectangleShape rectangle(Table[m_type].size);
  rectangle.setFillColor(Table[m_type].color);
  center_origin(rectangle);

  target.draw(rectangle, states);
}
