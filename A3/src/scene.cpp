#include "Scene.h"
#include "GameEngine.h"

Scene::Scene() = default;

Scene::Scene(GameEngine* game_engine) : m_game(game_engine)
{
}

Scene::~Scene() = default;

void Scene::do_action(const Action& action)
{
    s_do_action(action);
}

void Scene::set_paused(bool paused)
{
    m_paused = paused;
}

void Scene::simulate(const size_t frames)
{
}

void Scene::register_action(int input_key, Action::Name action_name)
{
    m_action_map[input_key] = action_name;
}

size_t Scene::width() const
{
    return m_game->window().getSize().x;
}

size_t Scene::height() const
{
    return m_game->window().getSize().y;
}

size_t Scene::current_frame() const
{
    return m_current_frame;
}

bool Scene::has_ended() const
{
    return m_has_ended;
}

const ActionMap& Scene::get_action_map() const
{
    return m_action_map;
}

void Scene::draw_line(const vec2& p1, const vec2& p2)
{
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(p1.x, p1.y)), sf::Vertex(sf::Vector2f(p2.x, p2.y))};

    m_game->window().draw(line, 2, sf::Lines);
}
