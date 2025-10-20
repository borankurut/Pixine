#pragma once

#include "entity.h"
#include "vec2.h"

class Physics
{
   public:
    vec2 get_overlap(Entity a, Entity b);

    vec2 get_previous_overlap(Entity a, Entity b);
};
