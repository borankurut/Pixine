#include <iostream>

#include "SFML//Window/Event.hpp"
// #include "Scene_Menu.h"
#include "Scene_Play.h"
#include "Assets.h"
// #include "Physics.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include "SFML/Graphics/RectangleShape.hpp"

Scene_Play::Scene_Play(GameEngine* game_engine, const std::string& level_path)
    : Scene(game_engine), m_level_path(level_path)
{
    init(level_path);
}

void Scene_Play::init(const std::string& level_path)
{
    register_action(sf::Keyboard::P, "PAUSE");
    register_action(sf::Keyboard::Escape, "QUIT");
    register_action(sf::Keyboard::T, "TOGGLE_TEXTURE");    // Toggle drawing (T)extures
    register_action(sf::Keyboard::C, "TOGGLE_COLLISION");  // Toggle drawing (C)ollision Boxes
    register_action(sf::Keyboard::G, "TOGGLE_GRID");       // Toggle drawing (G)rid

    // TODO: Register all other gameplay Actions
    // registerAction(sf::Keyboard::W, "JUMP");

    m_grid_text.setCharacterSize(12);
    m_grid_text.setFont(m_game->assets().get_font("Mario"));
    // m_gridText.setFont(m_game->assets().getFont("Tech"));

    load_level(level_path);
}

vec2 Scene_Play::grid_to_mid_pixel(float grid_x, float grid_y, std::shared_ptr<Entity> entity)
{
    // TODO: This function takes in a grid (x,y) position and an Entity
    //       Return a vec2 indicating where the CENTER position of the Entity should be
    //       You must use the Entity's Animation size to position it correctly
    //       The size of the grid width and height is stored in m_gridSize.x and m_gridSize.y
    //       The bottom-left corner of the Animation should aligh with the bottom left of the grid
    //       cell

    return vec2(0, 0);
}

void Scene_Play::load_level(const std::string& file_name)
{
    // reset the entity manager every time we load a level
    m_entity_manager = EntityManager();

    // TODO: read in the level file and add the appropriate entities
    //       use the PlayerConfig struct m_playerConfig to store player properties
    //       this struct is defined at the top of Scene_Play.h

    // NOTE: all the code below is sample code which shows you how to
    //       set up and use entities with the new syntax, it should be removed

    spawn_player();

    // some sample entities
    auto brick = m_entity_manager.add_entity("tile");
    // IMPORTANT: always add the CAnimation component first so that gridToMidPixel can compute
    // correctly
    brick->add_component<C_Animation>(m_game->assets().get_animation("Brick"), true);
    brick->add_component<C_Transform>(vec2(96, 480));
    // NOTE: Your final code should position the entity with the grid x,y position read from the
    // file: brick->addComponent<CTransform>(gridToMidPixel(gridX, gridY, brick));

    if (brick->get_component<C_Animation>().animation.get_name() == "Brick")
    {
        std::cout << "This could be a good way of identifying if a tile is a brick!\n";
    }

    auto block = m_entity_manager.add_entity("tile");
    // block->addComponent<CAnimation>(m_game->assets().getAnimation("Block"), true);
    block->add_component<C_Animation>(m_game->assets().get_animation("Ground"), true);
    block->add_component<C_Transform>(vec2(224, 480));
    // add a bounding box, this will now show up if we press the 'C' key
    // block->addComponent<CBoundingBox>(m_game->assets().getAnimation("Block").getSize());
    block->add_component<C_BoundingBox>(m_game->assets().get_animation("Ground").get_size());

    auto question = m_entity_manager.add_entity("tile");
    question->add_component<C_Animation>(m_game->assets().get_animation("Ground"), true);
    question->add_component<C_Transform>(vec2(352, 480));

    // NOTE: THIS IS INCREDIBLY IMPORTANT PLEASE READ THIS EXAMPLE
    //       Components are now returned as references rather than pointers
    //       If you do not specify a reference variable type, it will COPY the component
    //       Here is an example:
    //
    //       This will COPY the transform into the variable 'transform1' - it is INCORRECT
    //       Any changes you make to transform1 will not be changed inside the entity
    //       auto transform1 = entity->get<CTransform>()
    //
    //       This will REFERENCE the transform with the variable 'transform2' - it is CORRECT
    //       Now any changes you make to transform2 will be changed inside the entity
    //       auto& transform2 = entity->get<CTransform>()
}

void Scene_Play::spawn_player()
{
    // here is a sample player entity which you can use to construct other entities
    m_player = m_entity_manager.add_entity("player");
    m_player->add_component<C_Animation>(m_game->assets().get_animation("Stand"), true);
    m_player->add_component<C_Transform>(vec2(224, 352));
    m_player->add_component<C_BoundingBox>(vec2(48, 48));

    // TODO: be sure to add the remaining components to the player
}

void Scene_Play::spawn_bullet(std::shared_ptr<Entity> entity)
{
    // TODO: this should spawn a bullet at the given entity, going in the direction the entity is
    // facing
}

void Scene_Play::update()
{
    m_entity_manager.update();

    // TODO: implement pause functionality

    s_movement();
    s_lifespan();
    s_collision();
    s_animation();
    s_render();
    // m_currentFrame++;
}

void Scene_Play::s_movement()
{
    // TODO: Implement player movement/jumping based on its CInput component
    // TODO: Implement gravity's effect on the player
    // TODO: Implement the maximum player speed in both X and Y directions
    // NOTE: Setting an entity's scale.x to -1/1 will make it face to the left/right
}

void Scene_Play::s_lifespan()
{
    // TODO: Check lifespan of entities the have them, and destroy them if the go over
}

void Scene_Play::s_collision()
{
    // REMEMBER: SFML's (0,0) position is in the TOP-LEFT corner
    //           This means jumping will have a negative y-component
    //           and gravity will have a positive y-component
    //           Also, something BELOW something else will hava a y value GREATER than it
    //           Also, something ABOVE something else will hava a y value LESS than it

    // TODO: Implement Physics::GetOverlap() function, use it inside this function

    // TODO: Implement bullet/tile collisions
    //       Destroy the tile if it has a Brick animation
    // TODO: Implement player/tile collisions and resolutions
    //       Update the CState component of the player to store whether
    //       it is currently on the ground or in the air. This will be
    //       used by the Animation system
    // TODO: Check to see if the player has fallen down a hole (y > height())
    // TODO: Don't let the player walk off the left side of the map
}

void Scene_Play::s_do_action(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "TOGGLE_TEXTURE")
        {
            m_draw_textures = !m_draw_textures;
        }
        else if (action.name() == "TOGGLE_COLLISION")
        {
            m_draw_collision = !m_draw_collision;
        }
        else if (action.name() == "TOGGLE_GRID")
        {
            m_draw_grid = !m_draw_grid;
        }
        else if (action.name() == "PAUSE")
        {
            set_paused(!m_paused);
        }
        else if (action.name() == "QUIT")
        {
            on_end();
        }
        else if (action.name() == "JUMP")
        {
            // m_player jump
        }
    }
    else if (action.type() == "END")
    {
    }
}

void Scene_Play::s_animation()
{
    // TODO: Complete the Animation class code first
}

void Scene_Play::on_end()
{
    // TODO: when the scene ends, change back to the MENU scene
    // use m_game->changeScene(correct params);
    // m_game->changeScene( "MENU", std::make_shared<Scene_Menu>(m_game));
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
