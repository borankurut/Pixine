#pragma once

#include <utility>
#include "animation.h"

class Component
{
   public:
    bool has = false;
};

class C_Transform : public Component
{
   public:
    vec2 pos = {0.0, 0.0};
    vec2 prev_pos = {0.0, 0.0};
    vec2 velocity = {0.0, 0.0};
    vec2 scale = {1.0, 1.0};
    float angle = 0;

    C_Transform() = default;

    explicit C_Transform(const vec2& p) : pos(p) {}

    C_Transform(const vec2& p, const vec2& sp, const vec2& sc, float a)
        : pos(p), prev_pos(p), velocity(sp), scale(sc), angle(a)
    {
    }
};

class C_Lifespan : public Component
{
   public:
    int lifespan = 0;
    int frame_created = 0;

    C_Lifespan() = default;

    explicit C_Lifespan(int duration, int frame) : lifespan(duration), frame_created(frame) {}
};

class C_Input : public Component
{
   public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool shoot = false;
    bool can_shoot = true;
    bool can_jump = true;

    C_Input() = default;
};

class C_BoundingBox : public Component
{
   public:
    vec2 size;
    vec2 half_size;

    C_BoundingBox() = default;

    explicit C_BoundingBox(const vec2& s) : size(s), half_size(s.x / 2.0f, s.y / 2.0f) {}
};

class C_Animation : public Component
{
   public:
    Animation animation;
    bool repeat = false;

    C_Animation() = default;

    C_Animation(Animation a, bool r) : animation(std::move(a)), repeat(r) {}
};

class C_Gravity : public Component
{
   public:
    float gravity = 0;

    C_Gravity() = default;

    explicit C_Gravity(float g) : gravity(g) {}
};

class C_State : public Component
{
   public:
    std::string state = "jumping";

    C_State() = default;

    explicit C_State(std::string s) : state(std::move(s)) {}
};
