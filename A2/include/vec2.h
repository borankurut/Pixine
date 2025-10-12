#pragma once
#include <cmath>

class vec2
{
   public:
    float x = 0;
    float y = 0;

    vec2();
    vec2(float x, float y) : x(x), y(y) {}

    bool operator==(const vec2& rhs) const { return this->x == rhs.x && this->y == rhs.y; }
    bool operator!=(const vec2& rhs) const { return this->x != rhs.x || this->y != rhs.y; };

    vec2 operator+(const vec2& rhs) const { return vec2(this->x + rhs.x, this->y + rhs.y); };
    vec2 operator-(const vec2& rhs) const { return vec2(this->x - rhs.x, this->y - rhs.y); };
    vec2 operator/(const float value) const { return vec2(this->x / value, this->y / value); };
    vec2 operator*(const float value) const { return vec2(this->x * value, this->y * value); };

    void operator+=(const vec2& rhs)
    {
        this->x += rhs.x;
        this->y += rhs.y;
    };
    void operator-=(const vec2& rhs)
    {
        this->x -= rhs.x;
        this->y -= rhs.y;
    };
    void operator/=(const float value)
    {
        this->x /= value;
        this->y /= value;
    }
    void operator*=(const float value)
    {
        this->x *= value;
        this->y *= value;
    }

    float dist(const vec2& rhs) const { return std::sqrt(sqr_dist(rhs)); }

    float sqr_dist(const vec2& rhs) const
    {
        return (this->x - rhs.x) * (this->x - rhs.x) + (this->y - rhs.y) * (this->y - rhs.y);
    }
};
