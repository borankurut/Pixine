#pragma once

#include <string>
#include <tuple>

#include "Components.h"

class EntityManager;

typedef std::tuple<C_Transform, C_Lifespan, C_Input, C_BoundingBox, C_Animation, C_Gravity, C_State>
    ComponentTuple;

class Entity
{
    friend class EntityManager;

   public:
    void destroy();

    [[nodiscard]] size_t id() const;
    [[nodiscard]] bool is_active() const;
    [[nodiscard]] const std::string& tag() const;

    template <class T>
    bool has_component() const
    {
        return get_component<T>().has;
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
        return std::get<T>(m_components);
    }

    template <class T>
    const T& get_component() const
    {
        return std::get<T>(m_components);
    }

    template <class T>
    void remove_component() const
    {
        get_component<T>() = T();
    }

   private:
    bool m_active = true;
    const size_t m_id = 0;
    const std::string m_tag = "default";
    ComponentTuple m_components;

    // constructor is private, so we can never create entities
    // outside the EntityManager which had friend access
    Entity(size_t id, std::string tag);
};
