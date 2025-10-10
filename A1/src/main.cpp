#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/Font.hpp"
#include "SFML/Graphics/Glsl.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"

struct WindowData
{
    int w, h;
};

struct FontData
{
    std::string font_file_name;
    int size;
    int r, g, b;  // color
};

struct RectangleData
{
    std::string shape_name;
    float x, y;    // initial pos
    float sx, sy;  // initial speed
    int r, g, b;   // color
    int w, h;      // size
};

struct CircleData
{
    std::string shape_name;
    float x, y;    // initial pos
    float sx, sy;  // initial speed
    int r, g, b;   // color
    float radius;
};

WindowData read_window(std::ifstream& input)
{
    WindowData wd;
    input >> wd.w >> wd.h;
    return wd;
}

FontData read_font(std::ifstream& input)
{
    FontData fd;
    input >> fd.font_file_name >> fd.size >> fd.r >> fd.g >> fd.b;
    return fd;
}

RectangleData read_rectangle(std::ifstream& input)
{
    RectangleData rd;
    input >> rd.shape_name >> rd.x >> rd.y >> rd.sx >> rd.sy >> rd.r >> rd.g >> rd.b >> rd.w >>
        rd.h;
    return rd;
}

CircleData read_circle(std::ifstream& input)
{
    CircleData cd;
    input >> cd.shape_name >> cd.x >> cd.y >> cd.sx >> cd.sy >> cd.r >> cd.g >> cd.b >> cd.radius;
    return cd;
}

void render();

struct AppData
{
    sf::Font font;
    sf::Color font_color;
    float font_size;
    WindowData wd;
    std::unique_ptr<sf::RenderWindow> window;
    std::vector<sf::CircleShape> circles;
    std::vector<sf::RectangleShape> rectangles;
    std::vector<sf::Text> texts;
    std::vector<sf::Vector2f> velocities;
};

AppData g_app_data;

void process_config();
void update_physics();
void update_positions();
void update_collisions();

int main(int argc, char* argv[])
{
    process_config();
    g_app_data.window->setFramerateLimit(60);

    // position the top-left corner of the text so that it aligns on the bottom
    // text.setPosition(0, wHeight - (float)text.getCharacterSize());

    // main loop – continues for each frame while window is open
    std::cout << "hello " << std::endl;
    while (g_app_data.window->isOpen())
    {
        // event handling
        sf::Event event;
        while (g_app_data.window->pollEvent(event))
        {
            // this event triggers when the window is closed
            if (event.type == sf::Event::Closed)
            {
                g_app_data.window->close();
            }

            // this event is triggered when a key is pressed
            if (event.type == sf::Event::KeyPressed)
            {
                // print the key that was pressed to the console
                std::cout << "Key pressed with code = " << event.key.code << "\n";
            }
        }

        update_physics();

        // basic rendering function calls
        render();
    }

    return 0;
}

void render()
{
    g_app_data.window->clear();  // clear the window of anything previously drawn
                                 //

    for (int i = 0; i < g_app_data.circles.size(); i++)
    {
        g_app_data.window->draw(g_app_data.circles[i]);
        g_app_data.window->draw(g_app_data.texts[i]);
    }

    for (int i = 0; i < g_app_data.rectangles.size(); i++)
    {
        g_app_data.window->draw(g_app_data.rectangles[i]);
        g_app_data.window->draw(g_app_data.texts[i + g_app_data.circles.size()]);
    }

    g_app_data.window->display();  // call the window display function
}

void update_positions()
{
    for (int i = 0; i < g_app_data.circles.size(); ++i)
    {
        sf::Vector2f pos = g_app_data.circles[i].getPosition();
        sf::Vector2f pos_t = g_app_data.texts[i].getPosition();
        sf::Vector2f velocity = g_app_data.velocities[i];
        g_app_data.circles[i].setPosition(pos.x + velocity.x, pos.y + velocity.y);
        g_app_data.texts[i].setPosition(pos_t.x + velocity.x, pos_t.y + velocity.y);
    }
    for (int i = 0; i < g_app_data.rectangles.size(); ++i)
    {
        sf::Vector2f pos = g_app_data.rectangles[i].getPosition();
        sf::Vector2f pos_t = g_app_data.texts[i + g_app_data.circles.size()].getPosition();
        sf::Vector2f velocity = g_app_data.velocities[i + g_app_data.circles.size()];
        g_app_data.rectangles[i].setPosition(pos.x + velocity.x, pos.y + velocity.y);
        g_app_data.texts[i + g_app_data.circles.size()].setPosition(
            pos_t.x + velocity.x, pos_t.y + velocity.y);
    }
}

enum class Hit
{
    None = 0,
    Vertical,
    Horizontal
};

Hit check_collision(const sf::FloatRect& global_bounds)
{
    std::cout << g_app_data.window->getSize().x << ", ";
    std::cout << g_app_data.window->getSize().y << ", ";
    std::cout << global_bounds.top + global_bounds.height << ", ";
    std::cout << global_bounds.left + global_bounds.width << ", ";
    std::cout << std::endl;
    if (global_bounds.top < 0 || global_bounds.top + global_bounds.height > g_app_data.wd.h)
    {
        return Hit::Vertical;
    }
    else if (global_bounds.left < 0 || global_bounds.left + global_bounds.width > g_app_data.wd.w)
    {
        return Hit::Horizontal;
    }
    return Hit::None;
}

void update_collisions()
{
    for (int i = 0; i < g_app_data.circles.size(); i++)
    {
        sf::CircleShape& circle = g_app_data.circles[i];

        Hit hit = check_collision(circle.getGlobalBounds());
        if (hit == Hit::Vertical)
        {
            g_app_data.velocities[i].y *= -1.0f;
        }
        else if (hit == Hit::Horizontal)
        {
            g_app_data.velocities[i].x *= -1.0f;
        }
    }
    for (int i = 0; i < g_app_data.rectangles.size(); i++)
    {
        sf::RectangleShape& rectangle = g_app_data.rectangles[i];

        Hit bound = check_collision(rectangle.getGlobalBounds());
        if (bound == Hit::Vertical)
        {
            g_app_data.velocities[i + g_app_data.circles.size()].y *= -1.0f;
        }
        else if (bound == Hit::Horizontal)
        {
            g_app_data.velocities[i + g_app_data.circles.size()].x *= -1.0f;
        }
    }
}

void update_physics()
{
    update_collisions();
    update_positions();
}

void process_config()
{
    std::vector<CircleData> circleData;
    std::vector<RectangleData> rectangleData;

    std::string option;
    std::ifstream file("config.txt");
    while (file >> option)
    {
        if (option == "Window")
        {
            g_app_data.wd = read_window(file);
            if (!g_app_data.window)
            {
                g_app_data.window = std::make_unique<sf::RenderWindow>(
                    sf::VideoMode(g_app_data.wd.w, g_app_data.wd.h), "A1");
            }
        }
        else if (option == "Font")
        {
            FontData fd = read_font(file);
            g_app_data.font.loadFromFile(fd.font_file_name);
            g_app_data.font_color = sf::Color(fd.r, fd.g, fd.b);
            g_app_data.font_size = fd.size;
        }
        else if (option == "Circle")
        {
            circleData.push_back(read_circle(file));
        }
        else if (option == "Rectangle")
        {
            rectangleData.push_back(read_rectangle(file));
        }
    }

    for (const CircleData& cd : circleData)
    {
        sf::CircleShape circle;
        circle.setPosition(cd.x, cd.y);
        circle.setFillColor(sf::Color(cd.r, cd.g, cd.b));
        circle.setRadius(cd.radius);
        g_app_data.circles.push_back(circle);
        g_app_data.velocities.push_back(sf::Vector2f(cd.sx, cd.sy));

        sf::Text text(cd.shape_name, g_app_data.font, g_app_data.font_size);
        text.setFillColor(g_app_data.font_color);

        sf::Vector2f bounding_center = {
            circle.getGlobalBounds().left + circle.getGlobalBounds().width / 2.0f,
            circle.getGlobalBounds().top + circle.getGlobalBounds().height / 2.0f};

        sf::Vector2f top_left_pos = {
            bounding_center.x - text.getLocalBounds().width / 2.0f,
            bounding_center.y - text.getLocalBounds().height / 2.0f};

        text.setPosition(top_left_pos);
        g_app_data.texts.push_back(text);
    }

    for (const RectangleData& rd : rectangleData)
    {
        sf::RectangleShape rectangle;
        rectangle.setPosition(rd.x, rd.y);
        rectangle.setFillColor(sf::Color(rd.r, rd.g, rd.b));
        rectangle.setSize(sf::Vector2f(rd.w, rd.h));
        g_app_data.rectangles.push_back(rectangle);
        g_app_data.velocities.push_back(sf::Vector2f(rd.sx, rd.sy));

        sf::Text text(rd.shape_name, g_app_data.font, g_app_data.font_size);
        text.setFillColor(g_app_data.font_color);

        sf::Vector2f bounding_center = sf::Vector2f{
            rectangle.getGlobalBounds().left + rectangle.getGlobalBounds().width / 2.0f,
            rectangle.getGlobalBounds().top + rectangle.getGlobalBounds().height / 2.0f};

        sf::Vector2f top_left_pos = {
            bounding_center.x - text.getLocalBounds().width / 2.0f,
            bounding_center.y - text.getLocalBounds().height / 2.0f};

        text.setPosition(top_left_pos);
        g_app_data.texts.push_back(text);
    }
    g_app_data.window->setTitle("A1");
}
