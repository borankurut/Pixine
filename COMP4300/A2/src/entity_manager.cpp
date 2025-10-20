#include "entity_manager.hpp"

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
    for (auto entity : m_entities_to_add)
    {
        m_entities.push_back(entity);
        m_entity_map[entity->tag()].push_back(entity);
    }
    m_entities_to_add.resize(0);

    // remove dead entities from the vector of all entities
    remove_dead_entities(m_entities);

    // remove dead entities from eachh vector in teh entity map
    // c++17 way of iterating through [key, value] pairs in a map
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

const EntityVec& EntityManager::get_entities(const std::string& tag)
{
    return m_entity_map[tag];
}

const EntityVec& EntityManager::get_entities()
{
    return m_entities;
}
