#pragma once

#include <memory>
#include <vector>
#include <map>

#include "Entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EntityManager
{
    EntityVec m_entities;         // all entities
    EntityVec m_entities_to_add;  // entities to add next update
    EntityMap m_entity_map;       //  map from entity tag to vectors
    size_t m_total_entities = 0;  // total entities created

    // helper function to avoid repeated code
    void remove_dead_entities(EntityVec& vec);

   public:
    EntityManager();

    void update();

    std::shared_ptr<Entity> add_entity(const std::string& tag);

    EntityVec& get_entities();

    EntityVec& get_entities(const std::string& tag);

    const EntityMap& get_entity_map();
};
