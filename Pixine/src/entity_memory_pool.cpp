#include "entity_memory_pool.h"
#include <cassert>
#include "entity.h"

EntityMemoryPool::EntityMemoryPool(size_t max_entities)
{
    m_num_entities = 0;
    std::get<std::vector<C_Transform>>(m_data).resize(max_entities);
    std::get<std::vector<C_Lifespan>>(m_data).resize(max_entities);
    std::get<std::vector<C_Input>>(m_data).resize(max_entities);
    std::get<std::vector<C_BoundingBox>>(m_data).resize(max_entities);
    std::get<std::vector<C_Animation>>(m_data).resize(max_entities);
    std::get<std::vector<C_Gravity>>(m_data).resize(max_entities);
    std::get<std::vector<C_State>>(m_data).resize(max_entities);
    m_tags.resize(max_entities);
    m_active.resize(max_entities);
}

EntityMemoryPool& EntityMemoryPool::Instance()
{
    static EntityMemoryPool pool(MAX_ENTITIES);
    return pool;
}

Entity EntityMemoryPool::add_entity(const std::string& tag)
{
    size_t id = get_next_id();
    assert(id != MAX_ENTITIES);

    m_num_entities++;
    m_tags[id] = tag;
    m_active[id] = true;
    reset_components(id);

    return Entity(id);
}

void EntityMemoryPool::reset_components(size_t id)
{
    std::get<std::vector<C_Transform>>(m_data)[id].has = false;
    std::get<std::vector<C_Lifespan>>(m_data)[id].has = false;
    std::get<std::vector<C_Input>>(m_data)[id].has = false;
    std::get<std::vector<C_BoundingBox>>(m_data)[id].has = false;
    std::get<std::vector<C_Animation>>(m_data)[id].has = false;
    std::get<std::vector<C_Gravity>>(m_data)[id].has = false;
    std::get<std::vector<C_State>>(m_data)[id].has = false;
}

void EntityMemoryPool::remove_entity(size_t id)
{
    m_active[id] = false;
    m_tags[id] = "default";
    m_removed_entity_indices.push(id);
    m_num_entities--;
}

size_t EntityMemoryPool::get_next_id()
{
    if (!m_removed_entity_indices.empty())
    {
        size_t id = m_removed_entity_indices.top();
        m_removed_entity_indices.pop();
        return id;
    }

    if (m_num_entities < MAX_ENTITIES)
    {
        return m_num_entities;
    }

    return MAX_ENTITIES;
}
