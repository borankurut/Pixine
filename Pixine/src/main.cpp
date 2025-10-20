#include <SFML/Graphics.hpp>
#include "game_engine.h"

int main()
{
    GameEngine g("assets.txt");
    g.run();
    return 0;
}
