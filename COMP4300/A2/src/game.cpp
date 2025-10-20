#include "game.hpp"
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

    m_font.loadFromFile(m_font_config.F);
    m_text.setFont(m_font);
    m_text.setPosition(0, 0);
    m_text.setCharacterSize(m_font_config.S);
    m_text.setFillColor(sf::Color(m_font_config.R, m_font_config.G, m_font_config.B));

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
        handle_game_over();
        s_enemy_spawner();
        s_movement();
        s_collision();
        s_user_input();
        s_lifespan();
        update_score();
        s_render();
        m_current_frame++;
    }
}

void Game::update_score()
{
    m_text.setString("SCORE:  " + std::to_string(m_score));
}

void Game::set_paused(bool paused)
{
    m_paused = paused;
}

void Game::spawn_player()
{
    auto entity = m_entities.add_entity("player");
    entity->c_transform = std::make_shared<CTransform>(
        vec2(m_window_config.W / 2.0f, m_window_config.H / 2.0f), vec2(0.0f, 0.0f), 0.0f);

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
    auto enemy = m_entities.add_entity("enemy");

    vec2 pos = vec2(
        util::random(0 + m_enemy_config.SR, m_window_config.W - m_enemy_config.SR),
        util::random(0 + m_enemy_config.SR, m_window_config.H - m_enemy_config.SR));
    float speed = util::random(m_enemy_config.SMIN, m_enemy_config.SMAX);
    vec2 vel = vec2(util::random(-1.0f, 1.0f), util::random(-1.0f, 1.0f)).normalized() * speed;
    float angle = 0.0f;

    enemy->c_transform = std::make_shared<CTransform>(pos, vel, angle);

    enemy->c_shape = std::make_shared<CShape>(
        m_enemy_config.SR, util::random(m_enemy_config.VMIN, m_enemy_config.VMAX),
        util::random_color(), sf::Color(m_enemy_config.OR, m_enemy_config.OG, m_enemy_config.OB),
        m_enemy_config.OT);

    enemy->c_collision = std::make_shared<CCollision>(m_enemy_config.CR);

    enemy->c_score = std::make_shared<CScore>(enemy->c_shape->circle.getPointCount() * 100);

    m_last_enemy_spawn_time = m_current_frame;
}

// spawns the small enemies when a big one  (input entity e) explodes
void Game::spawn_small_enemies(std::shared_ptr<Entity> e)
{
    for (int i = 0; i < e->c_shape->circle.getPointCount(); i++)
    {
        auto small_enemy = m_entities.add_entity("small");

        small_enemy->c_shape = std::make_shared<CShape>(
            e->c_shape->circle.getRadius() / 2.0f, e->c_shape->circle.getPointCount(),
            e->c_shape->circle.getFillColor(), e->c_shape->circle.getOutlineColor(),
            e->c_shape->circle.getOutlineThickness());

        small_enemy->c_collision = std::make_shared<CCollision>(e->c_collision->radius / 2.0f);

        float dir_as_angle =
            ((float)e->c_transform->angle +
             ((float)i / (float)e->c_shape->circle.getPointCount()) * 360.0f);

        vec2 dir = vec2(
                       std::cos(dir_as_angle * (3.14159f / 180.0f)),
                       std::sin(dir_as_angle * (3.14159f / 180.0f)))
                       .normalized();

        small_enemy->c_transform = std::make_shared<CTransform>(
            e->c_transform->pos + dir * e->c_collision->radius,
            dir * e->c_transform->velocity.length(), e->c_transform->angle);

        small_enemy->c_lifespan = std::make_shared<CLifespan>(m_enemy_config.L);

        small_enemy->c_score = std::make_shared<CScore>(e->c_score->score * 2);
    }
}

// spawns a bullet from a given entity to a target location
void Game::spawn_bullet(std::shared_ptr<Entity> entity, const vec2& target)
{
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

    bullet->c_lifespan = std::make_shared<CLifespan>(m_bullet_config.L);
}

void Game::spawn_special_weapon(std::shared_ptr<Entity> entity)
{
    for (int i = 0; i < 10; i++)
    {
        float angle = i * ((2.0f * 3.14159f) / 10.0f);
        vec2 dir = vec2(std::sin(angle), std::cos(angle)).normalized();
        vec2 target = vec2(entity->c_transform->pos + dir * m_bullet_config.S * 3.0f);
        spawn_bullet(entity, target);
    }
}

void Game::s_movement()
{
    m_player->c_transform->velocity =
        vec2(
            (int)m_player->c_input->right - (int)m_player->c_input->left,
            (int)m_player->c_input->down - (int)m_player->c_input->up)
            .normalized() *
        m_player_config.S;

    if ((m_player->c_transform->pos.x - m_player->c_collision->radius < 0 &&
         m_player->c_input->left) ||
        (m_player->c_transform->pos.x + m_player->c_collision->radius > m_window_config.W &&
         m_player->c_input->right))
    {
        m_player->c_transform->velocity.x = 0;
    }

    if ((m_player->c_transform->pos.y - m_player->c_collision->radius < 0 &&
         m_player->c_input->up) ||
        (m_player->c_transform->pos.y + m_player->c_collision->radius > m_window_config.H &&
         m_player->c_input->down))
    {
        m_player->c_transform->velocity.y = 0;
    }

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
            auto& color_fill = entity->c_shape->circle.getFillColor();
            entity->c_shape->circle.setFillColor(
                sf::Color(
                    color_fill.r, color_fill.g, color_fill.b,
                    ((float)entity->c_lifespan->remaining / (float)entity->c_lifespan->total) *
                        255.0f));

            auto& color_outline = entity->c_shape->circle.getOutlineColor();
            entity->c_shape->circle.setOutlineColor(
                sf::Color(
                    color_outline.r, color_outline.g, color_outline.b,
                    ((float)entity->c_lifespan->remaining / (float)entity->c_lifespan->total) *
                        255.0f));
        }
    }
}

void Game::s_collision()
{
    for (auto enemy : m_entities.get_entities("enemy"))
    {
        if (handle_enemy_collision(enemy))
        {
            spawn_small_enemies(enemy);
        }
    }

    for (auto small_enemy : m_entities.get_entities("small"))
    {
        handle_enemy_collision(small_enemy);
    }
}

bool Game::handle_enemy_collision(std::shared_ptr<Entity> enemy)
{
    if (!enemy->c_collision)
    {
        return false;
    }

    bool result = false;

    handle_window_collision(enemy);

    for (auto bullet : m_entities.get_entities("bullet"))
    {
        if (check_collision(bullet, enemy))
        {
            m_score += enemy->c_score->score;
            enemy->destroy();
            bullet->destroy();
            result = true;
        }
    }
    if (check_collision(enemy, m_player))
    {
        enemy->destroy();
        handle_hit(enemy);
        result = true;
    }

    return result;
}

void Game::handle_game_over()
{
    if (m_lives <= 0)
    {
        m_lives = 3;
        m_score = 0;
        for (auto entity : m_entities.get_entities())
        {
            entity->destroy();
        }
        spawn_player();
    }
}

void Game::handle_hit(std::shared_ptr<Entity> enemy)
{
    vec2 pos = m_player->c_transform->pos;
    vec2 e_pos = enemy->c_transform->pos;
    constexpr float push_magnitude = 80.0f;
    vec2 dir = (pos - e_pos).normalized();
    m_player->c_transform->pos = pos + dir * push_magnitude;
    m_lives -= 1;
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
    m_window.draw(m_text);

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
                    m_player->c_input->up = true;
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
                    m_player->c_input->up = false;
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
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                spawn_special_weapon(m_player);
            }
        }
    }
}
