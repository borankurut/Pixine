#include <iostream>
#include <utility>

#include "SFML/Window/Event.hpp"
#include "GameEngine.h"
#include "Assets.h"
#include "Scene_Menu.h"
#include "Scene_Play.h"

GameEngine::GameEngine(const std::string& path)
{
    init(path);
}

void GameEngine::init(const std::string& path)
{
    // loading textures call glFlush(), so we need to create the window first
    m_window.create(sf::VideoMode(1280, 768), "Definitely Not Mario");
    m_window.setFramerateLimit(60);

    m_assets.load_from_file(path);

    change_scene("MENU", std::make_shared<Scene_Menu>(this));
}

std::shared_ptr<Scene> GameEngine::current_scene()
{
    return m_scene_map[m_current_scene];
}

bool GameEngine::is_running()
{
    return m_running;
}

sf::RenderWindow& GameEngine::window()
{
    return m_window;
}

void GameEngine::run()
{
    while (is_running())
    {
        s_user_input();
        update();
        m_window.display();
    }
}

void GameEngine::s_user_input()
{
    sf::Event event{};

    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            quit();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::X)
            {
                std::cout << "Save screenshot to " << "test.png" << std::endl;
                sf::Texture texture;
                texture.create(m_window.getSize().x, m_window.getSize().y);
                texture.update(m_window);
                if (texture.copyToImage().saveToFile("test.png"))
                {
                    std::cout << "Screenshot saved to " << "test.png" << std::endl;
                }
            }
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            // if the current scene does not have an action associated with this key, skip the event
            if (current_scene()->get_action_map().count(event.key.code) == 0)
            {
                std::cout << "DON'T KNOW WHAT TO DO.\n";
                continue;
            }

            // determine start or end action by whether it was key press or release
            const Action::Type action_type =
                (event.type == sf::Event::KeyPressed) ? Action::Type::START : Action::Type::END;

            // look up the action and send the action to the scene
            current_scene()->do_action(
                Action(current_scene()->get_action_map().at(event.key.code), action_type));
        }
    }
}

void GameEngine::change_scene(
    const std::string& scene_name, std::shared_ptr<Scene> scene, bool end_current_scene)
{
    m_current_scene = scene_name;
    m_scene_map[scene_name] = scene;
}

void GameEngine::quit()
{
    m_running = false;
    m_window.close();
}

void GameEngine::update()
{
    current_scene()->update();
}

const Assets& GameEngine::assets() const
{
    return m_assets;
}
