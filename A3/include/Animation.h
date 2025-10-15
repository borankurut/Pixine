#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "vec2.h"

class Animation
{
   public:
    Animation();
    Animation(const std::string& name, const sf::Texture& t);
    Animation(std::string name, const sf::Texture& t, size_t frame_count, size_t speed);

    void update();
    bool has_ended() const;

    const std::string& get_name() const;
    const vec2& get_size() const;

    sf::Sprite& sprite();

   private:
    sf::Sprite m_sprite;
    size_t m_frame_count = 1;    // total number of frames of animation
    size_t m_current_frame = 0;  // the current frame of animation being played
    size_t m_speed = 0;          // the speed to play this animation (inverse)
    vec2 m_size = {1, 1};        // size of the animation frame
    std::string m_name = "none";
};
