#pragma once
#include <SFML/Graphics.hpp>

namespace util
{
float random();
float random(float a, float b);
int random(int a, int b);
sf::Color random_color();
}  // namespace util
