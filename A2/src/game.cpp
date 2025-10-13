#include "game.hpp"
#include <iostream>  // TODO: remove this
#include <fstream>
#include "util.hpp"

Game::Game(const std::string& config)
{
    init(config);
}

void Game::init(const std::string& path)
{
    std::ifstream input(path);
    std::string key;
    input >> key >> m_window_config.W >> m_window_config.H >> m_window_config.FL >>
        m_window_config.FS;
    input >> key >> m_font_config.F >> m_font_config.S >> m_font_config.R >> m_font_config.G >>
        m_font_config.B;
    input >> key >> m_player_config.SR >> m_player_config.CR >> m_player_config.S >>
        m_player_config.FR >> m_player_config.FG >> m_player_config.FB >> m_player_config.OR >>
        m_player_config.OG >> m_player_config.OB >> m_player_config.OT >> m_player_config.V;
    input >> key >> m_enemy_config.SR >> m_enemy_config.CR >> m_enemy_config.SMIN >>
        m_enemy_config.SMAX >> m_enemy_config.OR >> m_enemy_config.OG >> m_enemy_config.OB >>
        m_enemy_config.OT >> m_enemy_config.VMIN >> m_enemy_config.VMAX >> m_enemy_config.L >>
        m_enemy_config.SI;
    input >> key >> m_bullet_config.SR >> m_bullet_config.CR >> m_bullet_config.S >>
        m_bullet_config.FR >> m_bullet_config.FG >> m_bullet_config.FB >> m_bullet_config.OR >>
        m_bullet_config.OG >> m_bullet_config.OB >> m_bullet_config.OT >> m_bullet_config.V >>
        m_bullet_config.L;

    // setup default windows parameters
    if (m_window_config.FS == 1)
    {
        m_window.create(
            sf::VideoMode(m_window_config.W, m_window_config.H), "Assignment 2",
            sf::Style::Fullscreen);
    }
    else
    {
        m_window.create(
            sf::VideoMode(m_window_config.W, m_window_config.H), "Assignment 2",
            sf::Style::Default);
    }
    m_window.setFramerateLimit(m_window_config.FL);

    spawn_player();
}

void Game::run()
{
    while (m_running)
    {
        if (m_paused)
        {
            s_user_input();
            s_render();
            continue;
        }

        m_entities.update();
        s_enemy_spawner();
        s_movement();
        s_collision();
        s_user_input();
        s_render();
        m_current_frame++;
    }
}

void Game::set_paused(bool paused)
{
    m_paused = paused;
}

void Game::spawn_player()
{
    auto entity = m_entities.add_entity("player");
    entity->c_transform =
        std::make_shared<CTransform>(vec2(200.0f, 200.0f), vec2(1.0f, 1.0f), 0.0f);

    entity->c_shape = std::make_shared<CShape>(
        m_player_config.SR, m_player_config.V,
        sf::Color(m_player_config.FR, m_player_config.FG, m_player_config.FB),
        sf::Color(m_player_config.OR, m_player_config.OG, m_player_config.OB), m_player_config.OT);

    entity->c_input = std::make_shared<CInput>();

    entity->c_collision = std::make_shared<CCollision>(m_player_config.CR);

    m_player = entity;
}

void Game::spawn_enemy()
{
    // TODO: make sure the enemy is spawned properly with the m_enemy_config variables
    // the enemy must be spawned completely within the bounds of the window

    auto entity = m_entities.add_entity("enemy");

    vec2 pos = vec2(
        util::random(0 + m_enemy_config.SR, m_window_config.W - m_enemy_config.SR),
        util::random(0 + m_enemy_config.SR, m_window_config.H - m_enemy_config.SR));
    float speed = util::random(m_enemy_config.SMIN, m_enemy_config.SMAX);
    vec2 vel = vec2(util::random(-1.0f, 1.0f), util::random(-1.0f, 1.0f)).normalized() * speed;
    float angle = 0.0f;

    entity->c_transform = std::make_shared<CTransform>(pos, vel, angle);

    entity->c_shape = std::make_shared<CShape>(
        m_enemy_config.SR, util::random(m_enemy_config.VMIN, m_enemy_config.VMAX),
        util::random_color(), sf::Color(m_enemy_config.OR, m_enemy_config.OG, m_enemy_config.OB),
        m_enemy_config.OT);

    entity->c_collision = std::make_shared<CCollision>(m_enemy_config.CR);

    m_last_enemy_spawn_time = m_current_frame;
}

// spawns the small enemies when a big one  (input entity e) explodes
void Game::spawn_small_enemies(std::shared_ptr<Entity> e)
{
    // TODO: spawn small enemied at the location of the input enemy e

    // when we create the smaller enemy, we have to read the valuues of the original enemy
    //  - spawn a number of small enemies equal to the vertices of the origginal enemy
    //  - set each small enemy to the same color as the original, half the size
    //  - small enemies are worth double points of the original enemy
}

// spawns a bullet from a given entity to a target location
void Game::spawn_bullet(std::shared_ptr<Entity> entity, const vec2& target)
{
    // TODO: implement the spawning of a bullet which travels toward target
    //  -- bullet speed is given as a scalar speed
    //  - you must set the velocity by using formula

    auto bullet = m_entities.add_entity("bullet");
    vec2 dir = (target - entity->c_transform->pos).normalized();
    vec2 velocity = dir * m_bullet_config.S;

    bullet->c_transform = std::make_shared<CTransform>(
        entity->c_transform->pos + dir * (entity->c_collision->radius + 0.001f), velocity, 0.0f);

    bullet->c_shape = std::make_shared<CShape>(
        m_bullet_config.SR, m_bullet_config.V,
        sf::Color(m_bullet_config.FR, m_bullet_config.FG, m_bullet_config.FB),
        sf::Color(m_bullet_config.OR, m_bullet_config.OG, m_bullet_config.OB), m_bullet_config.OT);

    bullet->c_collision = std::make_shared<CCollision>(m_bullet_config.CR);
}

void Game::spawn_special_weapon(std::shared_ptr<Entity> entity)
{
    // TODO: implement youur own special weapon
}

void Game::s_movement()
{
    m_player->c_transform->velocity =
        vec2(
            (int)m_player->c_input->right - (int)m_player->c_input->left,
            (int)m_player->c_input->down - (int)m_player->c_input->up)
            .normalized() *
        m_player_config.S;

    for (auto entity : m_entities.get_entities())
    {
        if (!entity->c_transform)
        {
            continue;
        }
        entity->c_transform->angle += 1.0f;
        entity->c_transform->pos.x += entity->c_transform->velocity.x;
        entity->c_transform->pos.y += entity->c_transform->velocity.y;
    }
}

void Game::s_lifespan()
{
    for (auto& entity : m_entities.get_entities())
    {
        if (entity->c_lifespan == nullptr)
        {
            continue;
        }

        entity->c_lifespan->remaining -= 1;
        if (entity->c_lifespan->remaining <= 0)
        {
            entity->destroy();
        }
        else
        {
            auto& color = entity->c_shape->circle.getFillColor();
            entity->c_shape->circle.setFillColor(
                sf::Color(
                    color.r, color.g, color.b,
                    entity->c_lifespan->remaining / entity->c_lifespan->total));
        }
    }
}

void Game::s_collision()
{
    for (auto enemy : m_entities.get_entities("enemy"))
    {
        if (!enemy->c_collision)
        {
            continue;
        }
        handle_window_collision(enemy);
        for (auto bullet : m_entities.get_entities("bullet"))
        {
            if (check_collision(bullet, enemy))
            {
                enemy->destroy();
            }
        }
    }
}

bool Game::check_collision(std::shared_ptr<Entity> entity_l, std::shared_ptr<Entity> entity_r)
{
    if (!entity_l->c_collision || !entity_r->c_collision || !entity_l->c_transform ||
        !entity_r->c_transform)
    {
        return false;
    }

    vec2 l_pos = entity_l->c_transform->pos;
    vec2 r_pos = entity_r->c_transform->pos;
    float l_radius = entity_l->c_collision->radius;
    float r_radius = entity_r->c_collision->radius;

    return l_pos.dist(r_pos) <= (l_radius + r_radius);
}

void Game::handle_window_collision(std::shared_ptr<Entity> entity)
{
    vec2 pos = entity->c_transform->pos;
    float radius = entity->c_collision->radius;
    if (pos.x - radius < 0 || pos.x + radius > m_window_config.W)
    {
        entity->c_transform->velocity.x *= -1;
    }

    if (pos.y - radius < 0 || pos.y + radius > m_window_config.H)
    {
        entity->c_transform->velocity.y *= -1;
    }
}

void Game::s_enemy_spawner()
{
    if (m_current_frame - m_last_enemy_spawn_time > m_enemy_config.SI)
    {
        spawn_enemy();
    }
}

void Game::s_render()
{
    m_window.clear();

    for (auto entity : m_entities.get_entities())
    {
        if (!entity->c_transform || !entity->c_shape)
        {
            continue;
        }

        // set the posiion of the shape based on the entity's transform->pos
        entity->c_shape->circle.setPosition(entity->c_transform->pos.x, entity->c_transform->pos.y);

        // set the rotation of the shape based on the entity's transform->angle
        entity->c_shape->circle.setRotation(entity->c_transform->angle);

        // draw the entity's sf::CircleShape
        m_window.draw(entity->c_shape->circle);
    }

    m_window.display();
}

void Game::s_user_input()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // this event triggers when the window is ccclosed
        if (event.type == sf::Event::Closed)
        {
            m_running = false;
        }

        // this event is triggered when a key is pressed
        if (event.type == sf::Event::KeyPressed)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                {
                    std::cout << "W key is pressed" << std::endl;
                    m_player->c_input->up = true;
                    // input up true
                }
                break;
                case sf::Keyboard::A:
                {
                    m_player->c_input->left = true;
                }
                break;
                case sf::Keyboard::S:
                {
                    m_player->c_input->down = true;
                }
                break;
                case sf::Keyboard::D:
                {
                    m_player->c_input->right = true;
                }
                break;

                case sf::Keyboard::Q:
                case sf::Keyboard::Escape:
                {
                    m_running = false;
                }
                break;

                case sf::Keyboard::P:
                {
                    m_paused = !m_paused;
                }
                break;

                default:
                    break;
            }
        }

        // this event is triggered when a key is released
        if (event.type == sf::Event::KeyReleased)
        {
            switch (event.key.code)
            {
                case sf::Keyboard::W:
                {
                    // input up false
                    m_player->c_input->up = false;
                    std::cout << "W key is released" << std::endl;
                }
                break;
                case sf::Keyboard::A:
                {
                    m_player->c_input->left = false;
                }
                break;
                case sf::Keyboard::S:
                {
                    m_player->c_input->down = false;
                }
                break;
                case sf::Keyboard::D:
                {
                    m_player->c_input->right = false;
                }
                break;

                default:
                    break;
            }
        }

        if (event.type == sf::Event::MouseButtonPressed)
        {
            if (event.mouseButton.button == sf::Mouse::Left)
            {
                spawn_bullet(m_player, vec2(event.mouseButton.x, event.mouseButton.y));
                std::cout << "Left Mouse Button Clicked at (" << event.mouseButton.x << ", "
                          << event.mouseButton.y << ")" << std::endl;
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                spawn_special_weapon(m_player);
                std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << ", "
                          << event.mouseButton.y << ")" << std::endl;
            }
        }
    }
}
