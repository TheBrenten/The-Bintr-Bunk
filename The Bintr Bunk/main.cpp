
#define say(x) std::cout<<x<<'\n'
#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "BintrsTools.h"
#include "GameFunction.h"
#include "MenuFunctions.h"
//#include "MenuObjects.h"

#include "AnimFunc.h"
#include "PhysFunc.h"
#include "RenderFunc.h"
using namespace std;

sf::RenderWindow* window = nullptr;

int resx = 900;
int resy = 500;

float physRate = 60;
float FPS = 60;

// create GameFunction objects

GameFunction gfPhysics(physFunc, 1 / physRate);
GameFunction gfRender (renderFunc, 1 / FPS);
GameFunction gfAnim   (animFunc, 1 / 30.f);

#include "Player.h"
#include "LoadResources.h"
Player* p;






int main()
{
    // randomy randomness
    std::srand(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count());

    // create window
    updateWindow();

    // player relies on resources, load them first
    loadResources();
    //mainMenu();
    goSingleplayer();

    while (window->isOpen()) {
        // Determine which game function to run next
        GameFunction* nextFunc = getNextFunction();        
        // Wait until next GF is ready, then run
        sf::sleep(sf::seconds(nextFunc->rate - nextFunc->clock.elapsed()));
        nextFunc->funcP();
        // Important to reset clock AFTER function runs, not before
        nextFunc->clock.reset();
    }
    return 0;
}