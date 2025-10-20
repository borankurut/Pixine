#include <iostream>

#include "SFML/Window/Event.hpp"
#include "scene_menu.h"
#include "scene_play.h"
#include "physics.h"
#include "assets.h"
#include "game_engine.h"
#include "components.h"
#include "action.h"

Scene_Menu::Scene_Menu(GameEngine* game_engine) : Scene(game_engine)
{
    init();
}

void Scene_Menu::init()
{
    register_action(sf::Keyboard::W, Action::Name::UP);
    register_action(sf::Keyboard::S, Action::Name::DOWN);
    register_action(sf::Keyboard::D, Action::Name::PLAY);
    register_action(sf::Keyboard::Escape, Action::Name::QUIT);

    m_title = "Lunatics";
    int title_size = 30;

    m_menu_text.setString(m_title);
    m_menu_text.setFont(m_game->assets().get_font("Mario"));
    m_menu_text.setCharacterSize(title_size);
    m_menu_text.setFillColor(sf::Color::Black);
    m_menu_text.setPosition(
        float(m_game->window().getSize().x) / 2.0f -
            float(title_size * (m_title.length() + 1)) / 2.0f,
        float(title_size * 3));

    m_menu_strings.emplace_back("LEVEL 1");
    m_menu_strings.emplace_back("LEVEL 2");
    m_menu_strings.emplace_back("LEVEL 3");

    for (int i = 0; i < m_menu_strings.size(); i++)
    {
        sf::Text text(m_menu_strings[i], m_game->assets().get_font("Mario"), 26);
        if (i != m_selected_menu_index)
        {
            text.setFillColor(sf::Color::Black);
        }
        text.setPosition(
            float(m_game->window().getSize().x) / 2.0f -
                float(26 * (m_menu_strings[i].length() + 1)) / 2.0f,
            m_menu_text.getGlobalBounds().top + 10.0f + 30.0f * float(i + 1));
        m_menu_items.push_back(text);
    }

    m_level_paths.emplace_back("level1.txt");
    m_level_paths.emplace_back("level2.txt");
    m_level_paths.emplace_back("level3.txt");
}

void Scene_Menu::update()
{
    // m_entityManager.update();
    s_render();
}

void Scene_Menu::on_end()
{
    m_game->quit();
}

void Scene_Menu::s_do_action(const Action& action)
{
    if (action.type() == Action::Type::START)
    {
        if (action.name() == Action::Name::UP)
        {
            if (m_selected_menu_index > 0)
            {
                m_selected_menu_index--;
            }
            else
            {
                m_selected_menu_index = m_menu_strings.size() - 1;
            }
        }
        else if (action.name() == Action::Name::DOWN)
        {
            m_selected_menu_index = (m_selected_menu_index + 1) % m_menu_strings.size();
        }
        else if (action.name() == Action::Name::PLAY)
        {
            m_game->change_scene(
                "PLAY", std::make_shared<Scene_Play>(m_game, m_level_paths[m_selected_menu_index]));
        }
        else if (action.name() == Action::Name::QUIT)
        {
            on_end();
        }
    }
}

void Scene_Menu::s_render()
{
    // set menu background
    m_game->window().clear(sf::Color(100, 100, 255));

    // draw title
    m_game->window().draw(m_menu_text);

    // draw menu items
    for (int i = 0; i < m_menu_strings.size(); i++)
    {
        if (i != m_selected_menu_index)
        {
            m_menu_items[i].setFillColor(sf::Color::Black);
        }
        else
        {
            m_menu_items[i].setFillColor(sf::Color::White);
        }

        m_menu_items[i].setPosition(
            float(m_game->window().getSize().x) / 2.0f -
                float(26 * (m_menu_strings[i].length() + 1)) / 2.0f,
            m_menu_text.getGlobalBounds().top + 10.0f + 30.0f * float(i + 1));
        m_game->window().draw(m_menu_items[i]);
    }

    // draw help
    sf::Text help("W:UP  S:DOWN  D:PLAY  ESC:BACK/QUIT", m_game->assets().get_font("Mario"), 26);
    help.setFillColor(sf::Color::Black);
    help.setPosition(
        float(m_game->window().getSize().x) / 2.0f -
            float(26 * (help.getString().getSize() + 1)) / 2.0f,
        float(m_game->window().getSize().y) - 30.0f * 2.0f);
    m_game->window().draw(help);
}
