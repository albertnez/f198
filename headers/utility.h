#ifndef UTILITY_H
#define UTILITY_H

#include <SFML/Window/Keyboard.hpp>
#include <SFML/System/Vector2.hpp>

#include <sstream>


namespace sf {
  class Sprite;
  class Text;
}

class Animation;

// Since std::to_string doesn't work on MinGW we have to implement
// our own to support all platforms.
template <typename T>
std::string to_string(const T& value);

// Convert enumerators to strings
std::string to_string(sf::Keyboard::Key key);

// Call setOrigin() with the center of the object
void center_origin(sf::Sprite& sprite);
void center_origin(sf::Text& text);
void center_origin(Animation& animation);

// Degree/radian conversion
float to_degree(float radian);
float to_radian(float degree);

// Random number generation
int random_int(int exclusiveMax);

// Vector operations
float length(sf::Vector2f vector);
sf::Vector2f unit_vector(sf::Vector2f vector);


#include "utility.inl"
#endif // UTILITY_H
