#include "Physics.h"

vec2 Physics::get_overlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    // TODO: return the overlap rectangle size of the bounding boxes of entity a and b
    return vec2(0, 0);
}

vec2 Physics::get_previous_overlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    // TODO: return the previous overlap rectangle size of the bounding boxes of entity a and b
    //       previous overlap uses the entity's previous position
    return vec2(0, 0);
}
