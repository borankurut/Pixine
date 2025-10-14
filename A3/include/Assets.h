#pragma once

#include <string>
#include <map>

#include "Animation.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

class Assets
{
    std::map<std::string, sf::Texture> m_texture_map;
    std::map<std::string, Animation> m_animation_map;
    std::map<std::string, sf::Font> m_font_map;

   public:
    Assets();

    void load_from_file(const std::string& path);

    void add_texture(const std::string& name, const std::string& path);

    void add_animation(const std::string& name, const Animation& animation);

    void add_font(const std::string& name, const std::string& path);

    const sf::Texture& get_texture(const std::string& name) const;

    const Animation& get_animation(const std::string& name) const;

    const sf::Font& get_font(const std::string& name) const;
};
