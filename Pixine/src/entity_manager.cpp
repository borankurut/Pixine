#include "entity_manager.h"

EntityManager::EntityManager() = default;

// called at beginning of each frame by game engine
// entities added will now be available to use this frame
void EntityManager::update()
{
    for (const auto& entity : m_entities_to_add)
    {
        m_entities.push_back(entity);
        m_entity_map[entity.tag()].push_back(entity);
    }
    m_entities_to_add.clear();

    remove_dead_entities(m_entities);

    for (auto& [tag, entityVec] : m_entity_map)
    {
        remove_dead_entities(entityVec);
    }
}

void EntityManager::remove_dead_entities(std::vector<Entity>& vec)
{
    size_t alive_count = 0;

    for (size_t i = 0; i < vec.size(); ++i)
    {
        if (vec[i].is_active())
        {
            vec[alive_count++] = vec[i];
        }
    }

    // can't do this because resize needs constructor
    // vec.resize(alive_count);

    vec.erase(vec.begin() + alive_count, vec.end());
}

Entity EntityManager::add_entity(const std::string& tag)
{
    Entity entity = EntityMemoryPool::Instance().add_entity(tag);
    m_entities_to_add.push_back(entity);
    return entity;
}

std::vector<Entity>& EntityManager::get_entities()
{
    return m_entities;
}

std::vector<Entity>& EntityManager::get_entities(const std::string& tag)
{
    return m_entity_map[tag];
}

const EntityMap& EntityManager::get_entity_map()
{
    return m_entity_map;
}
