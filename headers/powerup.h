#ifndef POWERUP_H
#define POWERUP_H

#include "entity.h"

class Ship;

class Powerup : public Entity {
public:
  enum Type {
    Heal,
    Bullet,
    FireRate,
    TypeCount,
  };
  explicit Powerup(Type type);

  void apply(Ship& ship);

private:
  Type m_type;

};

#endif // POWERUP_H
