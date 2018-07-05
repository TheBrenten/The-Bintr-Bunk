#pragma once
#include <vector>
#include "BintrsTools.h"
class GameFunction;
extern std::vector<GameFunction*> gameFunctions;

class GameFunction {
public:
    void(*funcP)();
    timer clock;
    float rate;
    GameFunction(void(*funcP)(), float rate) {
        gameFunctions.emplace_back(this);
        this->funcP = funcP;
        this->rate = rate;
    }
};


GameFunction* getNextFunction();