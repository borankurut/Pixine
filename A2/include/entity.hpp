#pragma once
#include "components.hpp"
#include <memory>
#include <string>

class Entity
{
    friend class EntityManager;

   public:
    std::shared_ptr<CTransform> c_transform;
    std::shared_ptr<CShape> c_shape;
    std::shared_ptr<CCollision> c_collision;
    std::shared_ptr<CInput> c_input;
    std::shared_ptr<CScore> c_score;
    std::shared_ptr<CLifespan> c_lifespan;

    bool is_active() const;
    const std::string& tag() const;
    const size_t id() const;
    void destroy();

   private:
    bool m_active = true;
    size_t m_id = 0;
    std::string m_tag = "default";

    Entity(const size_t id, const std::string& tag);
};
