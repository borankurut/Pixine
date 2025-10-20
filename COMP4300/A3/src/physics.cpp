#include "physics.h"

vec2 Physics::get_overlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    vec2 bb_half_a = a->get_component<C_BoundingBox>().half_size;
    vec2 bb_half_b = b->get_component<C_BoundingBox>().half_size;
    vec2 p_a = a->get_component<C_Transform>().pos;
    vec2 p_b = b->get_component<C_Transform>().pos;
    vec2 delta = vec2(std::abs(p_a.x - p_b.x), std::abs(p_a.y - p_b.y));

    return vec2(bb_half_a.x + bb_half_b.x - delta.x, bb_half_a.y + bb_half_b.y - delta.y);
}

vec2 Physics::get_previous_overlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    vec2 bb_half_a = a->get_component<C_BoundingBox>().half_size;
    vec2 bb_half_b = b->get_component<C_BoundingBox>().half_size;
    vec2 p_a = a->get_component<C_Transform>().prev_pos;
    vec2 p_b = b->get_component<C_Transform>().prev_pos;
    vec2 delta = vec2(std::abs(p_a.x - p_b.x), std::abs(p_a.y - p_b.y));

    return vec2(bb_half_a.x + bb_half_b.x - delta.x, bb_half_a.y + bb_half_b.y - delta.y);
}
