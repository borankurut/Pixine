#pragma once

#include <vector>
#include <map>

#include "entity.h"

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
    std::vector<Entity> m_entities;
    std::vector<Entity> m_entities_to_add;
    EntityMap m_entity_map;
    size_t m_total_entities = 0;
    void remove_dead_entities(std::vector<Entity>& vec);
};
