#pragma once

#include "entity.hpp"
#include "entity_manager.hpp"

#include <SFML/Graphics.hpp>

struct PlayerConfig
{
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V;
    float S;
};

struct EnemyConfig
{
    int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
    float SMIN, SMAX;
};

struct BulletConfig
{
    int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
    float S;
};

class Game
{
   public:
    Game(const std::string& config);
    void run();

   private:
    sf::RenderWindow m_window;
    EntityManager m_entities;
    sf::Font m_font;
    sf::Text m_text;  // the score text to be drawn to the screen
    PlayerConfig m_player_config;
    EnemyConfig m_enemy_config;
    BulletConfig m_bullet_config;
    int m_score = 0;
    int m_current_frame = 0;
    int m_last_enemy_spawn_time = 0;
    bool m_paused = false;  // whether update game logic
    bool m_running = true;  // whether the game is running
                            //
    std::shared_ptr<Entity> m_player;

    void init(const std::string& config);
    void set_paused(bool paused);

    void s_movement();
    void s_user_input();
    void s_lifespan();
    void s_render();
    void s_enemy_spawner();
    void s_collision();

    void spawn_player();
    void spawn_enemy();
    void spawn_small_enemies(std::shared_ptr<Entity> entity);
    void spawn_bullet(std::shared_ptr<Entity>, const vec2& mouse_pos);
    void spawn_special_weapon(std::shared_ptr<Entity> entity);
};
