#include <cassert>
#include <fstream>
#include <iostream>
#include "Assets.h"

Assets::Assets() = default;

void Assets::load_from_file(const std::string& path)
{
    std::ifstream file(path);
    if (!file)
    {
        std::cerr << "Could not load " << path << " file!\n";
        exit(-1);
    }

    std::string asset_type;
    while (file >> asset_type)
    {
        if (asset_type == "Texture")
        {
            std::string name;
            std::string image_file;
            file >> name >> image_file;
            add_texture(name, image_file);
        }
        else if (asset_type == "Animation")
        {
            std::string animation_name_str;
            std::string texture_name;
            int frames, speed;
            file >> animation_name_str >> texture_name >> frames >> speed;
            const sf::Texture& tex = get_texture(texture_name);
            Animation::Name animation_name = Animation::str_to_name(animation_name_str);
            add_animation(animation_name, Animation(animation_name, tex, frames, speed));
        }
        else if (asset_type == "Font")
        {
            std::string font_name;
            std::string font_path;
            file >> font_name >> font_path;
            add_font(font_name, font_path);
        }
        else
        {
            std::cerr << "Incorrect asset type: " << asset_type << "\n";
            exit(-1);
        }
    }
}

void Assets::add_texture(const std::string& name, const std::string& path)
{
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        std::cerr << "Could not load image: " << path << "!\n";
        exit(-1);
    }
    m_texture_map[name] = texture;
}

void Assets::add_animation(Animation::Name name, const Animation& animation)
{
    m_animation_map[name] = animation;
}

void Assets::add_font(const std::string& name, const std::string& path)
{
    sf::Font font;
    if (!font.loadFromFile(path))
    {
        std::cerr << "Could not load font: " << path << "\n";
        exit(-1);
    }
    m_font_map[name] = font;
}

const sf::Texture& Assets::get_texture(const std::string& name) const
{
    assert(m_texture_map.find(name) != m_texture_map.end());
    return m_texture_map.at(name);
}

const Animation& Assets::get_animation(Animation::Name name) const
{
    assert(m_animation_map.find(name) != m_animation_map.end());
    return m_animation_map.at(name);
}

const sf::Font& Assets::get_font(const std::string& name) const
{
    assert(m_font_map.find(name) != m_font_map.end());
    return m_font_map.at(name);
}
