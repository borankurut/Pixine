#pragma once

#include <vector>
#include <string>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "vec2.h"

class Animation
{
   public:
    enum class Name
    {
        NONE,
        STAND,
        STAND_SHOOT,
        RUN,
        RUN_SHOOT,
        JUMP,
        AIR_SHOOT,
        BUSTER,
        EXPLOSION,
        GROUND,
        BRICK,
        BRICK_DEBRIS,
        QUESTION,
        QUESTION_HIT,
        COIN_SPIN,
        TREE,
        CASTLE_SMALL,
        FLOWER,
        BUSH,
    };

    Animation();
    Animation(Animation::Name name, const sf::Texture& t);
    Animation(Animation::Name name, const sf::Texture& t, size_t frame_count, size_t speed);

    void update();
    bool has_ended() const;
    static std::string name_to_str(Animation::Name name);
    static Animation::Name str_to_name(const std::string& str);

    Animation::Name get_name() const;
    const vec2& get_size() const;

    sf::Sprite& sprite();

   private:
    sf::Sprite m_sprite;
    size_t m_frame_count = 1;    // total number of frames of animation
    size_t m_current_frame = 0;  // the current frame of animation being played
    size_t m_speed = 0;          // the speed to play this animation (inverse)
    vec2 m_size = {1, 1};        // size of the animation frame
    Animation::Name m_name = Animation::Name::NONE;
};
