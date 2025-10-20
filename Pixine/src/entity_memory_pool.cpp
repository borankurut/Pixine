#include "entity_memory_pool.h"
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
    size_t index = get_next_index();
    m_tags[index] = tag;
    m_active[index] = true;

    std::get<std::vector<C_Transform>>(m_data)[index].has = false;
    std::get<std::vector<C_Lifespan>>(m_data)[index].has = false;
    std::get<std::vector<C_Input>>(m_data)[index].has = false;
    std::get<std::vector<C_BoundingBox>>(m_data)[index].has = false;
    std::get<std::vector<C_Animation>>(m_data)[index].has = false;
    std::get<std::vector<C_Gravity>>(m_data)[index].has = false;
    std::get<std::vector<C_State>>(m_data)[index].has = false;

    return Entity(index);
    return Entity(index);
}

void EntityMemoryPool::remove_entity(size_t id)
{
    m_active[id] = false;
    m_tags[id] = "default";
}

size_t EntityMemoryPool::get_next_index()
{
    // TODO: add a removed entity look up.
    for (size_t i = 0; i < MAX_ENTITIES; ++i)
    {
        if (!m_active[i])
        {
            return i;
            m_num_entities++;
        }
    }
    return -1;
}
