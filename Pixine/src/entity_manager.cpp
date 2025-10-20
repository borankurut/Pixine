#include "EntityManager.h"

EntityManager::EntityManager() = default;

// called at beginning of each frame by game engine
// entities added will now be available to use this frame
void EntityManager::update()
{
    // Add entities from m_entitiesToAdd to the proper location(s):
    // - add them to the vector of all entities
    // - add them to the vector inside the map, with the tag as a key
    for (const auto& entity : m_entities_to_add)
    {
        m_entities.push_back(entity);
        m_entity_map[entity->tag()].push_back(entity);
    }
    m_entities_to_add.clear();

    // remove dead entities from the vector of all entities
    remove_dead_entities(m_entities);

    // remove dead entities from each vector in the entity map
    // C++20 way ot iterating through [key, value] pairs in a map
    for (auto& [tag, entityVec] : m_entity_map)
    {
        remove_dead_entities(entityVec);
    }
}

void EntityManager::remove_dead_entities(EntityVec& vec)
{
    size_t alive_count = 0;

    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (vec[i]->is_active())
        {
            vec[alive_count++] = vec[i];
        }
    }

    vec.resize(alive_count);
}

std::shared_ptr<Entity> EntityManager::add_entity(const std::string& tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_total_entities++, tag));
    m_entities_to_add.push_back(entity);

    return entity;
}

EntityVec& EntityManager::get_entities()
{
    return m_entities;
}

EntityVec& EntityManager::get_entities(const std::string& tag)
{
    return m_entity_map[tag];
}

const EntityMap& EntityManager::get_entity_map()
{
    return m_entity_map;
}
