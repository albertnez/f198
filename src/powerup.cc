#include "powerup.h"
#include "ship.h"
#include "data_tables.h"

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

