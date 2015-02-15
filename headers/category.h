#ifndef CATEGORY_H
#define CATEGORY_H


// Entity/scene node category, used to dispatch commands
namespace Category {

enum Type {
  None = 0,
  Player = 1 << 0,
  ObjectLayer = 1 << 1,
  AllyBullet = 1 << 2,
  EnemyBullet = 1 << 3
};

}

#endif // CATEGORY_H
