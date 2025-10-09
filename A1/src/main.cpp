#include <SFML/Graphics.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    // create a new window of size 640x480 pixels
    // top-left of the window is (0,0) and bottom-right is (w,h)
    const int wWidth = 640;
    const int wHeight = 480;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML works!");

    // let's make a shape that we will draw to the screen
    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(300.0f, 300.0f);
    float circleMoveSpeed = 0.01f;

    // let's load a font so we can display some text
    sf::Font myFont;
    if (!myFont.loadFromFile("fonts/tech.ttf"))
    {
        std::cerr << "Could not load font!\n";
        exit(-1);
    }

    // set up the text object that will be drawn to the screen
    sf::Text text("Sample Text", myFont, 24);

    // position the top-left corner of the text so that it aligns on the bottom
    text.setPosition(0, wHeight - (float)text.getCharacterSize());

    // main loop – continues for each frame while window is open
    while (window.isOpen())
    {
        // event handling
        sf::Event event;
        while (window.pollEvent(event))
        {
            // this event triggers when the window is closed
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            // this event is triggered when a key is pressed
            if (event.type == sf::Event::KeyPressed)
            {
                // print the key that was pressed to the console
                std::cout << "Key pressed with code = " << event.key.code << "\n";

                // example: what happens when X is pressed
                if (event.key.code == sf::Keyboard::X)
                {
                    // reverse the direction of the circle on the screen
                    circleMoveSpeed *= -1.0f;
                }
            }
        }

        // basic animation – move the circle each frame if it's still in frame
        circle.setPosition(
            circle.getPosition().x - circleMoveSpeed, circle.getPosition().y - circleMoveSpeed);

        // basic rendering function calls
        window.clear();       // clear the window of anything previously drawn
        window.draw(circle);  // draw the circle
        window.draw(text);    // draw the text
        window.display();     // call the window display function
    }

    return 0;
}
