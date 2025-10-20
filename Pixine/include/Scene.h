#pragma once

#include "Action.h"
#include "EntityManager.h"

class GameEngine;

typedef std::map<int, Action::Name> ActionMap;

class Scene
{
   protected:
    GameEngine* m_game = nullptr;
    EntityManager m_entity_manager;
    ActionMap m_action_map;
    bool m_paused = false;
    bool m_has_ended = false;
    size_t m_current_frame = 0;

    virtual void on_end() = 0;

    void set_paused(bool paused);

   public:
    Scene();

    explicit Scene(GameEngine* game_engine);

    virtual ~Scene();

    virtual void update() = 0;

    virtual void s_do_action(const Action& action) = 0;

    virtual void s_render() = 0;

    virtual void do_action(const Action& action);

    void simulate(size_t frames);

    void register_action(int input_key, Action::Name action_name);

    [[nodiscard]] size_t width() const;

    [[nodiscard]] size_t height() const;

    [[nodiscard]] size_t current_frame() const;

    [[nodiscard]] bool has_ended() const;

    [[nodiscard]] const ActionMap& get_action_map() const;

    void draw_line(const vec2& p1, const vec2& p2);
};
