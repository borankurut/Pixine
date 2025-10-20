#pragma once
#include <stack>
#include "components.h"

#define MAX_ENTITIES 10000

typedef std::tuple<
    std::vector<C_Transform>,    //
    std::vector<C_Lifespan>,     //
    std::vector<C_Input>,        //
    std::vector<C_BoundingBox>,  //
    std::vector<C_Animation>,    //
    std::vector<C_Gravity>,      //
    std::vector<C_State>>        //
    EntityComponentVectorTuple;

class Entity;
class EntityMemoryPool
{
   public:
    static EntityMemoryPool& Instance();

    template <typename T>
    T& get_component(size_t entity_id)
    {
        return std::get<std::vector<T>>(m_data)[entity_id];
    }

    template <typename T>
    const T& get_component(size_t entity_id) const
    {
        return std::get<std::vector<T>>(m_data)[entity_id];
    }

    template <typename T>
    bool has_component(size_t entity_id)
    {
        return std::get<std::vector<T>>(m_data)[entity_id].has;
    }

    const std::string& get_tag(size_t entity_id) { return m_tags[entity_id]; }

    bool get_active(size_t entity_id) { return m_active[entity_id]; }

    Entity add_entity(const std::string& tag);
    void remove_entity(size_t id);

   private:
    size_t m_num_entities;
    EntityComponentVectorTuple m_data;
    std::vector<std::string> m_tags;
    std::vector<bool> m_active;
    std::stack<size_t> m_removed_entity_indices;
    EntityMemoryPool(size_t max_entities);
    size_t get_next_id();
    void reset_components(size_t index);
};
