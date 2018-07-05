#include "GameFunction.h"


std::vector<GameFunction*> gameFunctions;

GameFunction * getNextFunction()
{
    GameFunction* nextFunc = nullptr;
    float lowestTime = 10;
    for (int i = 0; i < gameFunctions.size(); i++) {
        GameFunction* gf = gameFunctions[i];
        float timeUntilFunc = gf->rate - gf->clock.elapsed();
        if (timeUntilFunc < lowestTime) {
            nextFunc = gf;
            lowestTime = timeUntilFunc;
        }
    }
    return nextFunc;
}