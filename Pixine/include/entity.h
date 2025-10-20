#pragma once

#include <string>
#include "entity_memory_pool.h"

class Entity
{
    friend class EntityMemoryPool;

   public:
    size_t id() const;

    void destroy();

    [[nodiscard]] bool is_active() const;
    [[nodiscard]] const std::string& tag() const;

    template <class T>
    bool has_component() const
    {
        return EntityMemoryPool::Instance().has_component<T>(m_id);
    }

    template <class T, typename... TArgs>
    T& add_component(TArgs&&... mArgs)
    {
        auto& component = get_component<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template <class T>
    T& get_component()
    {
        return EntityMemoryPool::Instance().get_component<T>(m_id);
    }

    template <class T>
    const T& get_component() const
    {
        return EntityMemoryPool::Instance().get_component<T>(m_id);
    }

    template <class T>
    void remove_component()
    {
        get_component<T>() = T();
    }

    static Entity invalid_entity;

   private:
    size_t m_id;
    Entity(size_t id);
    Entity();
};
