#pragma once

#include <map>
#include <memory>

#include "SFML/Graphics/Text.hpp"
#include "Scene.h"
#include "EntityManager.h"

class Scene_Play : public Scene
{
    struct PlayerConfig
    {
        float X, Y, CX, CY, SPEED, MAX_SPEED, JUMP, GRAVITY;
        std::string WEAPON;
    };

   protected:
    std::shared_ptr<Entity> m_player;
    std::string m_level_path;
    PlayerConfig m_player_config;
    bool m_draw_textures = true;
    bool m_draw_collision = false;
    bool m_draw_grid = false;
    const vec2 m_grid_size = {64, 64};
    sf::Text m_grid_text;
    // Physics m_worldPhysics;

    void init(const std::string& level_path);

    vec2 grid_to_mid_pixel(float, float, std::shared_ptr<Entity>);

    void load_level(const std::string& file_name);

    void spawn_player();

    void spawn_bullet(std::shared_ptr<Entity> entity);

    void s_movement();

    void s_lifespan();

    void s_collision();

    void s_animation();

    void s_render() override;

    void s_do_action(const Action& action) override;

    void on_end() override;

    //    void changePlayerStateTo(PlayerState s);
    //    void spawnCoinSpin(std::shared_ptr<Entity> tile);
    //    void spawnBrickDebris(std::shared_ptr<Entity> tile);

   public:
    Scene_Play(GameEngine* game_engine, const std::string& level_path);

    void update() override;
};
