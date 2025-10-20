#include <iostream>
#include <fstream>

#include "SFML//Window/Event.hpp"
// #include "Scene_Menu.h"
#include "Scene_Play.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "Scene_Menu.h"

Scene_Play::Scene_Play(GameEngine* game_engine, const std::string& level_path)
    : Scene(game_engine), m_level_path(level_path)
{
    init(level_path);
}

void Scene_Play::init(const std::string& level_path)
{
    register_action(sf::Keyboard::P, Action::Name::PAUSE);
    register_action(sf::Keyboard::Escape, Action::Name::QUIT);
    register_action(sf::Keyboard::T, Action::Name::TOGGLE_TEXTURE);  // Toggle drawing (T)extures
    register_action(
        sf::Keyboard::C, Action::Name::TOGGLE_COLLISION);  // Toggle drawing (C)ollision Boxes
    register_action(sf::Keyboard::G, Action::Name::TOGGLE_GRID);  // Toggle drawing (G)rid
    register_action(sf::Keyboard::W, Action::Name::JUMP);
    register_action(sf::Keyboard::A, Action::Name::LEFT);
    register_action(sf::Keyboard::S, Action::Name::DOWN);
    register_action(sf::Keyboard::D, Action::Name::RIGHT);
    register_action(sf::Keyboard::Space, Action::Name::SHOOT);

    m_grid_text.setCharacterSize(12);
    m_grid_text.setFont(m_game->assets().get_font("Mario"));
    // m_gridText.setFont(m_game->assets().getFont("Tech"));

    load_level(level_path);
}

vec2 Scene_Play::grid_to_mid_pixel(float grid_x, float grid_y, std::shared_ptr<Entity> entity)
{
    int x = m_grid_size.x * grid_x;
    int y = m_grid_size.y * grid_y;

    C_Animation& anim = entity->get_component<C_Animation>();
    vec2 anim_size = vec2(anim.animation.get_size().x, anim.animation.get_size().y);

    x += anim_size.x / 2;
    y += anim_size.y / 2;
    y = height() - y;

    return vec2(x, y);
}

void Scene_Play::load_level(const std::string& file_name)
{
    // reset the entity manager every time we load a level
    m_entity_manager = EntityManager();

    std::ifstream level_file(file_name);
    std::string option = "";
    while (level_file >> option)
    {
        if (option == "Tile")
        {
            TileConfig tc;
            level_file >> tc.N >> tc.X >> tc.Y;
            spawn_tile(tc);
        }

        else if (option == "Dec")
        {
            DecorationConfig dc;
            level_file >> dc.N >> dc.X >> dc.Y;
            spawn_decoration(dc);
        }

        else if (option == "Player")
        {
            level_file >> m_player_config.X >> m_player_config.Y >> m_player_config.CW >>
                m_player_config.CH >> m_player_config.SX >> m_player_config.SY >>
                m_player_config.SM >> m_player_config.GY >> m_player_config.B;
        }
    }
    spawn_player();
}

void Scene_Play::spawn_tile(const TileConfig& tc)
{
    auto tile = m_entity_manager.add_entity("tile");
    tile->add_component<C_Animation>(
        m_game->assets().get_animation(Animation::str_to_name(tc.N)), true);
    tile->add_component<C_Transform>(grid_to_mid_pixel(tc.X, tc.Y, tile));
    tile->add_component<C_BoundingBox>(m_grid_size);
}

void Scene_Play::spawn_decoration(const DecorationConfig& dc)
{
    auto decoration = m_entity_manager.add_entity("decoration");
    decoration->add_component<C_Animation>(
        m_game->assets().get_animation(Animation::str_to_name(dc.N)), true);
    decoration->add_component<C_Transform>(grid_to_mid_pixel(dc.X, dc.Y, decoration));
}

void Scene_Play::spawn_player()
{
    m_player = m_entity_manager.add_entity("player");
    m_player->add_component<C_Animation>(
        m_game->assets().get_animation(Animation::Name::STAND), true);
    m_player->add_component<C_State>("ground");
    m_player->add_component<C_Transform>(
        grid_to_mid_pixel(m_player_config.X, m_player_config.Y, m_player));
    m_player->add_component<C_BoundingBox>(vec2(m_player_config.CW, m_player_config.CH));
    m_player->add_component<C_Input>();
    m_player->add_component<C_Gravity>(m_player_config.GY);
}

void Scene_Play::spawn_bullet(std::shared_ptr<Entity> entity)
{
    auto bullet = m_entity_manager.add_entity("bullet");
    bullet->add_component<C_Animation>(
        m_game->assets().get_animation(Animation::str_to_name(m_player_config.B)), true);
    bullet->add_component<C_Transform>(entity->get_component<C_Transform>().pos);
    bullet->get_component<C_Transform>().velocity =
        vec2(entity->get_component<C_Transform>().scale.x * 20, 0);
    bullet->add_component<C_Lifespan>((2 * 60), m_current_frame);
    bullet->add_component<C_BoundingBox>(vec2(10, 10));
}

void Scene_Play::update()
{
    m_entity_manager.update();

    if (m_paused)
    {
        s_render();
        return;
    }
    s_movement();
    s_lifespan();
    s_collision();
    s_animation();
    s_render();
    m_current_frame++;
}

void Scene_Play::s_movement()
{
    C_Input& input = m_player->get_component<C_Input>();
    C_Transform& p_transform = m_player->get_component<C_Transform>();
    C_Gravity& p_gravity = m_player->get_component<C_Gravity>();
    C_State& p_state = m_player->get_component<C_State>();

    if (input.right)
    {
        p_transform.velocity.x = m_player_config.SX;
        p_transform.scale.x = 1;
        p_state.state = "run";
    }
    else if (input.left)
    {
        p_transform.velocity.x = -m_player_config.SX;
        p_transform.scale.x = -1;
        p_state.state = "run";
    }
    else
    {
        p_state.state = "stand";
        p_transform.velocity.x = 0;
    }

    if (input.up && input.can_jump)
    {
        p_transform.velocity.y = -m_player_config.SY;
        input.can_jump = false;
    }
    if (!input.up && p_transform.velocity.y < 0)
    {
        p_transform.velocity.y = 0;
    }

    for (auto e : m_entity_manager.get_entities())
    {
        if (e->has_component<C_Transform>())
        {
            if (e->has_component<C_Gravity>())
            {
                p_transform.velocity.y += p_gravity.gravity;
            }

            C_Transform& t = e->get_component<C_Transform>();
            t.prev_pos = t.pos;
            t.pos += t.velocity;
        }
    }
}

void Scene_Play::s_lifespan()
{
    for (auto entity : m_entity_manager.get_entities())
    {
        if (entity->has_component<C_Lifespan>())
        {
            C_Lifespan& lifespan = entity->get_component<C_Lifespan>();
            if (m_current_frame - lifespan.frame_created > lifespan.lifespan)
            {
                entity->destroy();
            }
        }
    }
}

bool Scene_Play::resolve_vertical_collision_player(std::shared_ptr<Entity> tile, vec2 overlap)
{
    bool grounded = false;
    C_Transform& p_t = m_player->get_component<C_Transform>();
    C_Input& p_i = m_player->get_component<C_Input>();
    C_Transform& t_t = tile->get_component<C_Transform>();
    p_t.velocity.y = 0;

    if (p_t.pos.y < t_t.pos.y)
    {
        p_t.pos.y -= overlap.y;
        if (!p_i.up)
        {
            p_i.can_jump = true;
        }
        grounded = true;

        std::cout << "Collision from Below!" << std::endl;
    }
    else
    {
        p_t.pos.y += overlap.y;
        std::cout << "Collision from Above!" << std::endl;

        if (tile->get_component<C_Animation>().animation.get_name() == Animation::Name::QUESTION)
        {
            tile->add_component<C_Animation>(
                m_game->assets().get_animation(Animation::Name::QUESTION_HIT), true);

            auto coin = m_entity_manager.add_entity("coin");

            coin->add_component<C_Animation>(
                m_game->assets().get_animation(Animation::Name::COIN_SPIN), true);
            coin->add_component<C_Transform>(
                tile->get_component<C_Transform>().pos + vec2(0, -m_grid_size.y));
            coin->get_component<C_Transform>().scale.x =
                coin->get_component<C_Transform>().scale.y = 0.5;
            coin->add_component<C_BoundingBox>(vec2(10, 10));
        }
    }
    return grounded;
}

void Scene_Play::resolve_horizontal_collision_player(std::shared_ptr<Entity> tile, vec2 overlap)
{
    C_Transform& p_t = m_player->get_component<C_Transform>();
    C_Transform& t_t = tile->get_component<C_Transform>();
    p_t.velocity.x = 0;

    if (p_t.pos.x < t_t.pos.x)
    {
        p_t.pos.x -= (overlap.x);
        std::cout << "Collision from Right!" << std::endl;
    }
    else
    {
        p_t.pos.x += (overlap.x);
        std::cout << "Collision from Left!" << std::endl;
    }
}

void Scene_Play::s_collision()
{
    // REMEMBER: SFML's (0,0) position is in the TOP-LEFT corner
    //           This means jumping will have a negative y-component
    //           and gravity will have a positive y-component
    //           Also, something BELOW something else will hava a y value GREATER than it
    //           Also, something ABOVE something else will hava a y value LESS than it

    C_Transform& p_t = m_player->get_component<C_Transform>();
    bool air = true;

    for (auto tile : m_entity_manager.get_entities("tile"))
    {
        for (auto bullet : m_entity_manager.get_entities("bullet"))
        {
            Animation::Name name = tile->get_component<C_Animation>().animation.get_name();
            vec2 collision = m_world_physics.get_overlap(bullet, tile);
            if (collision.x > 0 && collision.y > 0)
            {
                bullet->destroy();

                if (name == Animation::Name::BRICK)
                {
                    tile->remove_component<C_BoundingBox>();
                    tile->add_component<C_Animation>(
                        m_game->assets().get_animation(Animation::Name::EXPLOSION), false);
                }
            }
        }

        //
        // TODO: Implement player/tile collisions and resolutions
        //       Update the CState component of the player to store whether
        //       it is currently on the ground or in the air. This will be
        //       used by the Animation system

        if (!tile->has_component<C_BoundingBox>())
        {
            continue;
        }
        vec2 overlap = m_world_physics.get_overlap(m_player, tile);

        if (overlap.x > 0 && overlap.y > 0)
        {
            vec2 p_overlap = m_world_physics.get_previous_overlap(m_player, tile);
            if (p_overlap.y <= 0)
            {
                if (resolve_vertical_collision_player(tile, overlap))
                {
                    air = false;
                }
            }
            else
            {
                resolve_horizontal_collision_player(tile, overlap);
            }
        }
    }

    if (air)
    {
        m_player->get_component<C_State>().state = "air";
    }

    if (p_t.pos.y > height())
    {
        load_level(m_level_path);
    }

    if (p_t.pos.x < 0)
    {
        p_t.pos.x = 0;
    }

    // coin collision
    for (auto coin : m_entity_manager.get_entities("coin"))
    {
        vec2 overlap = m_world_physics.get_overlap(coin, m_player);
        if (overlap.x > 0 && overlap.y > 0)
        {
            coin->destroy();
        }
    }
}

void Scene_Play::s_do_action(const Action& action)
{
    if (action.type() == Action::Type::START)
    {
        if (action.name() == Action::Name::TOGGLE_TEXTURE)
        {
            m_draw_textures = !m_draw_textures;
        }
        else if (action.name() == Action::Name::TOGGLE_COLLISION)
        {
            m_draw_collision = !m_draw_collision;
        }
        else if (action.name() == Action::Name::TOGGLE_GRID)
        {
            m_draw_grid = !m_draw_grid;
        }
        else if (action.name() == Action::Name::PAUSE)
        {
            set_paused(!m_paused);
        }
        else if (action.name() == Action::Name::QUIT)
        {
            on_end();
        }
        else if (action.name() == Action::Name::JUMP)
        {
            m_player->get_component<C_Input>().up = true;
        }
        else if (action.name() == Action::Name::LEFT)
        {
            m_player->get_component<C_Input>().left = true;
        }
        else if (action.name() == Action::Name::DOWN)
        {
            m_player->get_component<C_Input>().down = true;
        }
        else if (action.name() == Action::Name::RIGHT)
        {
            m_player->get_component<C_Input>().right = true;
        }
        else if (action.name() == Action::Name::SHOOT)
        {
            C_Input& p_input = m_player->get_component<C_Input>();
            const std::string& p_state = m_player->get_component<C_State>().state;
            p_input.shoot = true;
            if (p_state == "air")
            {
                m_player->add_component<C_Animation>(
                    m_game->assets().get_animation(Animation::Name::AIR_SHOOT), false);
            }
            else if (p_state == "run")
            {
                m_player->add_component<C_Animation>(
                    m_game->assets().get_animation(Animation::Name::RUN_SHOOT), false);
            }
            else
            {
                m_player->add_component<C_Animation>(
                    m_game->assets().get_animation(Animation::Name::STAND_SHOOT), false);
            }
            spawn_bullet(m_player);
        }
    }
    else if (action.type() == Action::Type::END)
    {
        if (action.name() == Action::Name::JUMP)
        {
            m_player->get_component<C_Input>().up = false;
        }
        else if (action.name() == Action::Name::LEFT)
        {
            m_player->get_component<C_Input>().left = false;
        }
        else if (action.name() == Action::Name::DOWN)
        {
            m_player->get_component<C_Input>().down = false;
        }
        else if (action.name() == Action::Name::RIGHT)
        {
            m_player->get_component<C_Input>().right = false;
        }
        else if (action.name() == Action::Name::SHOOT)
        {
            m_player->get_component<C_Input>().shoot = false;
            m_player->get_component<C_Input>().can_shoot = true;
        }
    }
}

void Scene_Play::s_animation()
{
    for (auto entity : m_entity_manager.get_entities())
    {
        C_Animation& anim = entity->get_component<C_Animation>();
        anim.animation.update();
        if (!anim.repeat && anim.animation.has_ended())
        {
            if (entity != m_player)
            {
                entity->destroy();
            }
        }
    }

    C_Animation& p_anim = m_player->get_component<C_Animation>();
    C_State& p_state = m_player->get_component<C_State>();
    if (p_anim.repeat == false && !p_anim.animation.has_ended())
    {
        return;
    }

    if (p_state.state == "stand")
    {
        if (p_anim.animation.get_name() != Animation::Name::STAND)
        {
            m_player->add_component<C_Animation>(
                m_game->assets().get_animation(Animation::Name::STAND), true);
        }
    }
    else if (p_state.state == "run")
    {
        if (p_anim.animation.get_name() != Animation::Name::RUN)
        {
            m_player->add_component<C_Animation>(
                m_game->assets().get_animation(Animation::Name::RUN), true);
        }
    }
    else if (p_state.state == "air")
    {
        if (p_anim.animation.get_name() != Animation::Name::JUMP)
        {
            m_player->add_component<C_Animation>(
                m_game->assets().get_animation(Animation::Name::JUMP), true);
        }
    }
}

void Scene_Play::on_end()
{
    m_game->change_scene("MENU", std::make_shared<Scene_Menu>(m_game));
}

void Scene_Play::s_render()
{
    // color the background darker, so you know that the game is paused
    if (!m_paused)
    {
        m_game->window().clear(sf::Color(100, 100, 255));
    }
    else
    {
        m_game->window().clear(sf::Color(50, 50, 150));
    }

    // set the viewport of the window to be centered on the player if it's far enough right
    auto& p_pos = m_player->get_component<C_Transform>().pos;
    float window_center_x = std::max(m_game->window().getSize().x / 2.0f, p_pos.x);
    sf::View view = m_game->window().getView();
    view.setCenter(window_center_x, m_game->window().getSize().y - view.getCenter().y);
    m_game->window().setView(view);

    // draw all Entity textures / animations
    if (m_draw_textures)
    {
        for (const auto& e : m_entity_manager.get_entities())
        {
            auto& transform = e->get_component<C_Transform>();
            if (e->has_component<C_Animation>())
            {
                auto& animation = e->get_component<C_Animation>().animation;
                animation.sprite().setRotation(transform.angle);
                animation.sprite().setPosition(transform.pos.x, transform.pos.y);
                animation.sprite().setScale(transform.scale.x, transform.scale.y);
                m_game->window().draw(animation.sprite());
            }
        }
    }

    // draw all Entity collision bounding boxes with a rectangle shape
    if (m_draw_collision)
    {
        for (const auto& e : m_entity_manager.get_entities())
        {
            if (e->has_component<C_BoundingBox>())
            {
                auto& box = e->get_component<C_BoundingBox>();
                auto& transform = e->get_component<C_Transform>();
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x - 1, box.size.y - 1));
                rect.setOrigin(sf::Vector2f(box.half_size.x, box.half_size.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0, 0, 0, 0));
                rect.setOutlineColor(sf::Color::White);
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }

    // draw the grid so that can easily debug
    if (m_draw_grid)
    {
        float left_x = m_game->window().getView().getCenter().x - width() / 2.0;
        float right_x = left_x + width() + m_grid_size.x;
        float next_grid_x = left_x - ((int)left_x % (int)m_grid_size.x);

        for (float x = next_grid_x; x < right_x; x += m_grid_size.x)
        {
            draw_line(vec2(x, 0), vec2(x, height()));
        }

        for (float y = 0; y < height(); y += m_grid_size.y)
        {
            draw_line(vec2(left_x, height() - y), vec2(right_x, height() - y));

            for (float x = next_grid_x; x < right_x; x += m_grid_size.x)
            {
                std::string x_cell = std::to_string((int)x / (int)m_grid_size.x);
                std::string y_cell = std::to_string((int)y / (int)m_grid_size.y);
                m_grid_text.setString("(" + x_cell + "," + y_cell + ")");
                m_grid_text.setPosition(x + 3, height() - y - m_grid_size.y + 2);
                m_game->window().draw(m_grid_text);
            }
        }
    }
}
