#ifndef RESOURCE_IDENTIFIERS_H
#define RESOURCE_IDENTIFIERS_H


// Forward declaration of SFML classes
namespace sf {
class Texture;
class Font;
class Shader;
}

// ID of textures
namespace Textures {
enum ID {
  Particle,
};
}

// ID of fonts
namespace Fonts {
enum ID {
  Main,
};
}

// ID of shaders
namespace Shaders {
enum ID {
  Background,
  Player,
  Enemy,
};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;
typedef ResourceHolder<sf::Shader, Shaders::ID> ShaderHolder;

#endif // RESOURCE_IDENTIFIERS_H
