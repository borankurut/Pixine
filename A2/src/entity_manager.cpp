#include "entity_manager.hpp"

EntityManager::EntityManager()
{
}

void EntityManager::update()
{
    // TODO: add entities from m_entities_to_add to the proper location(s)
    // - add them to the vector of all entities
    // - add them to the vector inside the map, with the tag aas a key

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
    // TODO: remove all dead entities from the input vector
    // this is called by the update() function
}

std::shared_ptr<Entity> EntityManager::add_entity(const std::string& tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_total_entities++, tag));

    m_entities.push_back(entity);

    return entity;
}

const EntityVec& EntityManager::get_entities(const std::string& tag)
{
    // TODO: this is incorrect, return the correct vector from the map
    return m_entities;
}
