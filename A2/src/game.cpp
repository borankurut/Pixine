#include "game.hpp"
#include <iostream>  // TODO: remove this

Game::Game(const std::string& config)
{
    init(config);
}

void Game::init(const std::string& path)
{
    // TODO: read in config file here
    // use the premade PlayerConfig...

    // setup default windows parameters
    m_window.create(sf::VideoMode(1280, 720), "Assignment 2");
    m_window.setFramerateLimit(60);

    spawn_player();
}

void Game::run()
{
    // TODO: add pause functionality in here

    while (m_running)
    {
        m_entities.update();
        s_enemy_spawner();
        s_movement();
        s_collision();
        s_user_input();
        s_render();

        // move when pause implemented
        m_current_frame++;
    }
}

void Game::set_paused(bool paused)
{
    // TODO
}

void Game::spawn_player()
{
    // TODO: Finish adding all properties of the player with the correct valuues from the config

    auto entity = m_entities.add_entity("player");
    entity->c_transform =
        std::make_shared<CTransform>(vec2(200.0f, 200.0f), vec2(1.0f, 1.0f), 0.0f);

    entity->c_shape =
        std::make_shared<CShape>(32.0f, 8, sf::Color(10, 10, 10), sf::Color(255, 0, 0), 4.0f);

    entity->c_input = std::make_shared<CInput>();

    m_player = entity;
}

void Game::spawn_enemy()
{
    // TODO: make sure the enemy is spawned properly with the m_enemy_config variables
    // thhe enemy must be spawned completely within the bounds of the window

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
}

void Game::spawn_special_weapon(std::shared_ptr<Entity> entity)
{
    // TODO: implement youur own special weapon
}

void Game::s_movement()
{
    // TODO: implement all entity movement in this function
    // you should read the m_player->c_input component to determine if the player is moving
    //

    // sample movement speed update
    m_player->c_transform->pos.x += m_player->c_transform->velocity.x;
    m_player->c_transform->pos.y += m_player->c_transform->velocity.y;
}

void Game::s_lifespan()
{
    // TODO: implement all lifespan functionality
    //
    // for all entities
    //		if entity has no lifespan component, skip it
    //		if entity has >0 remaining lifespan, suubtractt 1
    //		if it has lifespan and is alive
    //			scale its alpha channel properly
    //		if it has lifespan and its time is up
    //			destroy the entity
}

void Game::s_collision()
{
    // TODO: implement all proper collisions between entities
    // be sure to use the collision radius, NOT the shape radius
}

void Game::s_enemy_spawner()
{
    // TODO: code which implements enemy spawning shouuld go here
    //
    //		(use m_current_frame - m_last_enemy_spawn_time) to determine
    //		how long it has been sicne the last enemy spawned
}

void Game::s_render()
{
    // TODO: change the code below to draw ALL the entities
    // sample drawing of the player Entity that we have created

    m_window.clear();
    // set the posiion of the shape based on the entity's transform->pos
    m_player->c_shape->circle.setPosition(
        m_player->c_transform->pos.x, m_player->c_transform->pos.y);

    // set the rotation of the shape based on the entity's transform->angle
    m_player->c_transform->angle += 1.0f;
    m_player->c_shape->circle.setRotation(m_player->c_transform->angle);

    // draw the entity's sf::CircleShape
    m_window.draw(m_player->c_shape->circle);

    m_window.display();
}

void Game::s_user_input()
{
    // TODO: handle user input here
    // note that you should only be setting the player's input component variables here
    // you should not implement the player's movement logicc here
    // the movement system will read the variabgles you set in this function
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
                    // input up true
                }
                break;

                case sf::Keyboard::Q:
                case sf::Keyboard::Escape:
                {
                    m_running = false;
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
                    std::cout << "W key is released" << std::endl;
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
                // call spawn bullet here
                std::cout << "Left Mouse Button Clicked at (" << event.mouseButton.x << ", "
                          << event.mouseButton.y << ")" << std::endl;
            }
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                // call spawn special weapon here
                std::cout << "Right Mouse Button Clicked at (" << event.mouseButton.x << ", "
                          << event.mouseButton.y << ")" << std::endl;
            }
        }
    }
}
