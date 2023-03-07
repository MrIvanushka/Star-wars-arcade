#include "src/Engine/Game.h"

int main()
{
    Game game("SWTOR",
              1920, 1080,
              4, 4,
              false);

    //MAIN LOOP
    while (!game.getWindowShouldClose())
    {
        //UPDATE INPUT ---
        game.update();
        game.render();
    }

    return 0;
}


