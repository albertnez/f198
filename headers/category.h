#ifndef CATEGORY_H
#define CATEGORY_H


// Entity/scene node category, used to dispatch commands
namespace Category {

enum Type {
  None = 0,
  Player = 1 << 0,
  Enemy = 1 << 1,
  ObjectLayer = 1 << 2,
  AllyBullet = 1 << 3,
  EnemyBullet = 1 << 4,
};

}

#endif // CATEGORY_H
