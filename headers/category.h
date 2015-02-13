#ifndef CATEGORY_H
#define CATEGORY_H


// Entity/scene node category, used to dispatch commands
namespace Category {

enum Type {
  None = 0,
  Player = 1 << 0,
};

}

#endif // CATEGORY_H
