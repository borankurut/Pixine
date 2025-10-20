#pragma once

#include <map>
#include <memory>
#include <deque>

#include "SFML/Graphics/Text.hpp"
#include "scene.h"
#include "entity_manager.h"

class Scene_Menu : public Scene
{
   protected:
    std::string m_title;
    std::vector<std::string> m_menu_strings;

    sf::Text m_menu_text;
    std::vector<sf::Text> m_menu_items;

    std::vector<std::string> m_level_paths;
    size_t m_selected_menu_index = 0;

    void init();

    void update() override;

    void on_end() override;

    void s_do_action(const Action& action) override;

   public:
    explicit Scene_Menu(GameEngine* game_engine = nullptr);

    void s_render() override;
};
