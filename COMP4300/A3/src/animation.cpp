#include "Animation.h"
#include <cmath>

Animation::Animation() = default;

Animation::Animation(Animation::Name name, const sf::Texture& t) : Animation(name, t, 1, 0)
{
}

Animation::Animation(Animation::Name name, const sf::Texture& t, size_t frame_count, size_t speed)
    : m_name(name), m_sprite(t), m_frame_count(frame_count), m_current_frame(0), m_speed(speed)
{
    m_size = vec2((float)t.getSize().x / float(frame_count), (float)t.getSize().y);
    m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
    m_sprite.setTextureRect(sf::IntRect(0, 0, m_size.x, m_size.y));
}

// updates the animation to show the next frame, depending on its speed
// animation loops when it reaches the end
void Animation::update()
{
    m_current_frame++;
    int frame = (m_current_frame / m_speed) % m_frame_count;
    m_sprite.setTextureRect(
        sf::IntRect(std::floor(float(frame) * m_size.x), 0, m_size.x, m_size.y));
}

bool Animation::has_ended() const
{
    return (m_current_frame / m_speed) > m_frame_count;
}
std::string Animation::name_to_str(Animation::Name name)
{
    switch (name)
    {
        case Animation::Name::NONE:
            return "None";
        case Animation::Name::STAND:
            return "Stand";
        case Animation::Name::STAND_SHOOT:
            return "StandShoot";
        case Animation::Name::RUN:
            return "Run";
        case Animation::Name::RUN_SHOOT:
            return "RunShoot";
        case Animation::Name::JUMP:
            return "Jump";
        case Animation::Name::AIR_SHOOT:
            return "AirShoot";
        case Animation::Name::BUSTER:
            return "Buster";
        case Animation::Name::EXPLOSION:
            return "Explosion";
        case Animation::Name::GROUND:
            return "Ground";
        case Animation::Name::BRICK:
            return "Brick";
        case Animation::Name::BRICK_DEBRIS:
            return "BrickDebris";
        case Animation::Name::QUESTION:
            return "Question";
        case Animation::Name::QUESTION_HIT:
            return "QuestionHit";
        case Animation::Name::COIN_SPIN:
            return "CoinSpin";
        case Animation::Name::TREE:
            return "Tree";
        case Animation::Name::CASTLE_SMALL:
            return "CastleSmall";
        case Animation::Name::FLOWER:
            return "Flower";
        case Animation::Name::BUSH:
            return "Bush";
    };
}
Animation::Name Animation::str_to_name(const std::string& str)
{
    if (str == "Stand")
    {
        return Animation::Name::STAND;
    }
    else if (str == "StandShoot")
    {
        return Animation::Name::STAND_SHOOT;
    }
    else if (str == "Run")
    {
        return Animation::Name::RUN;
    }
    else if (str == "RunShoot")
    {
        return Animation::Name::RUN_SHOOT;
    }
    else if (str == "Jump")
    {
        return Animation::Name::JUMP;
    }
    else if (str == "AirShoot")
    {
        return Animation::Name::AIR_SHOOT;
    }
    else if (str == "Buster")
    {
        return Animation::Name::BUSTER;
    }
    else if (str == "Explosion")
    {
        return Animation::Name::EXPLOSION;
    }
    else if (str == "Ground")
    {
        return Animation::Name::GROUND;
    }
    else if (str == "Brick")
    {
        return Animation::Name::BRICK;
    }
    else if (str == "BrickDebris")
    {
        return Animation::Name::BRICK_DEBRIS;
    }
    else if (str == "Question")
    {
        return Animation::Name::QUESTION;
    }
    else if (str == "QuestionHit")
    {
        return Animation::Name::QUESTION_HIT;
    }
    else if (str == "CoinSpin")
    {
        return Animation::Name::COIN_SPIN;
    }
    else if (str == "Tree")
    {
        return Animation::Name::TREE;
    }
    else if (str == "CastleSmall")
    {
        return Animation::Name::CASTLE_SMALL;
    }
    else if (str == "Flower")
    {
        return Animation::Name::FLOWER;
    }
    else if (str == "Bush")
    {
        return Animation::Name::BUSH;
    }

    return Animation::Name::NONE;
}

const vec2& Animation::get_size() const
{
    return m_size;
}

Animation::Name Animation::get_name() const
{
    return m_name;
}

sf::Sprite& Animation::sprite()
{
    return m_sprite;
}
