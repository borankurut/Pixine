#pragma once

#include <memory>
#include "entity.h"
#include "vec2.h"

class Physics
{
   public:
    vec2 get_overlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

    vec2 get_previous_overlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
};
