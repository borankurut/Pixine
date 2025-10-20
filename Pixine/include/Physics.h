#pragma once

#include "Entity.h"
#include "vec2.h"

class Physics
{
   public:
    vec2 get_overlap(Entity a, Entity b);

    vec2 get_previous_overlap(Entity a, Entity b);
};
