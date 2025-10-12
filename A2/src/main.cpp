#include <SFML/Graphics.hpp>
#include "Game.hpp"

int main()
{
    Game game("config.txt");
    game.run();
    return 0;
}
