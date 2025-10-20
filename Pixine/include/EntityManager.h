#pragma once

#include <vector>
#include <map>

#include "Entity.h"

typedef std::map<std::string, std::vector<Entity>> EntityMap;

class EntityManager
{
   public:
    EntityManager();

    void update();

    Entity add_entity(const std::string& tag);

    std::vector<Entity>& get_entities();

    std::vector<Entity>& get_entities(const std::string& tag);

    const EntityMap& get_entity_map();

   private:
    std::vector<Entity> m_entities;         // all entities
    std::vector<Entity> m_entities_to_add;  // entities to add next update
    EntityMap m_entity_map;                 //  map from entity tag to vectors
    size_t m_total_entities = 0;            // total entities created
    void remove_dead_entities(std::vector<Entity>& vec);
};
