#include "entity.h"

Entity::Entity(const size_t id) : m_id(id)
{
    /*empty*/
}

Entity::Entity() : m_id(-1)
{
}

bool Entity::is_active() const
{
    return EntityMemoryPool::Instance().get_active(m_id);
}

const std::string& Entity::tag() const
{
    return EntityMemoryPool::Instance().get_tag(m_id);
}

size_t Entity::id() const
{
    return m_id;
}

void Entity::destroy()
{
    EntityMemoryPool::Instance().remove_entity(m_id);
}

Entity Entity::invalid_entity = Entity(-1);
