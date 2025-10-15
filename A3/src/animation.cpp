#include "Animation.h"
#include <cmath>
#include <utility>

Animation::Animation() = default;

Animation::Animation(const std::string& name, const sf::Texture& t) : Animation(name, t, 1, 0)
{
}

Animation::Animation(std::string name, const sf::Texture& t, size_t frame_count, size_t speed)
    : m_name(std::move(name)),
      m_sprite(t),
      m_frame_count(frame_count),
      m_current_frame(0),
      m_speed(speed)
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
    return (m_current_frame / m_speed) >= m_frame_count;
}

const vec2& Animation::get_size() const
{
    return m_size;
}

const std::string& Animation::get_name() const
{
    return m_name;
}

sf::Sprite& Animation::sprite()
{
    return m_sprite;
}
