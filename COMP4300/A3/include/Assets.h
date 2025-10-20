#pragma once

#include <string>
#include <map>

#include "Animation.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>

class Assets
{
   public:
    Assets();

    void load_from_file(const std::string& path);

    void add_texture(const std::string& name, const std::string& path);
    void add_animation(Animation::Name name, const Animation& animation);
    void add_font(const std::string& name, const std::string& path);

    const sf::Texture& get_texture(const std::string& name) const;
    const Animation& get_animation(Animation::Name name) const;
    const sf::Font& get_font(const std::string& name) const;

   private:
    std::map<std::string, sf::Texture> m_texture_map;
    std::map<Animation::Name, Animation> m_animation_map;
    std::map<std::string, sf::Font> m_font_map;
};
