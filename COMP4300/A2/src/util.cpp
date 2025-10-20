#include "util.hpp"
#include <random>

float util::random()
{
    static thread_local std::mt19937 generator{std::random_device{}()};
    static thread_local std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    return distribution(generator);
}

float util::random(float a, float b)
{
    float t = random();
    return (1 - t) * a + t * b;
    return (t * a) + ((1 - t) * b);
}

int util::random(int a, int b)
{
    return (int)random((float)a, (float)b);
}

sf::Color util::random_color()
{
    return sf::Color(random(0, 255), random(0, 255), random(0, 255));
}
