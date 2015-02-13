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
};
}

// ID of fonts
namespace Fonts {
enum ID {
  Main,
};
}

// Forward declaration and a few type definitions
template <typename Resource, typename Identifier>
class ResourceHolder;

typedef ResourceHolder<sf::Texture, Textures::ID> TextureHolder;
typedef ResourceHolder<sf::Font, Fonts::ID> FontHolder;

#endif // RESOURCE_IDENTIFIERS_H
