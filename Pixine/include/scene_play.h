#pragma once

#include "SFML/Graphics/Text.hpp"
#include "scene.h"
#include "physics.h"

class Scene_Play : public Scene
{
    struct TileConfig
    {
        std::string N;
        float X, Y;
    };

    struct DecorationConfig
    {
        std::string N;
        float X, Y;
    };

    struct PlayerConfig
    {
        float X, Y, CW, CH, SX, SY, SM, GY;
        std::string B;
    };

   public:
    Scene_Play(GameEngine* game_engine, const std::string& level_path);

    void update() override;

   protected:
    Entity m_player = Entity::invalid_entity;
    std::string m_level_path;
    PlayerConfig m_player_config;
    bool m_draw_textures = true;
    bool m_draw_collision = false;
    bool m_draw_grid = false;
    const vec2 m_grid_size = {64, 64};
    sf::Text m_grid_text;
    Physics m_world_physics;

    void init(const std::string& level_path);

    vec2 grid_to_mid_pixel(float, float, Entity);

    void load_level(const std::string& file_name);

    void spawn_tile(const TileConfig& tc);
    void spawn_decoration(const DecorationConfig& dc);
    void spawn_player();

    void spawn_bullet(Entity entity);

    void s_movement();

    void s_lifespan();

    // returns grounded (collision from below)
    bool resolve_vertical_collision_player(Entity tile, vec2 overlap);
    void resolve_horizontal_collision_player(Entity tile, vec2 overlap);
    void s_collision();

    void s_animation();

    void s_render() override;

    void s_do_action(const Action& action) override;

    void on_end() override;
};
