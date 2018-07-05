#pragma once
#include <vector>
#include "Region.h"
using namespace std;
#define say(x) std::cout<<x<<'\n'
class GameObject;
extern vector<GameObject*> activeObjects;

class GameObject
{
public:
    bool canMove = false;
    bool allowedToHearPlayer = false;

    vec pos;
    vec posLast;
    vec posInterp;
    vec posInterpLast;
    rect localHull;
    int life = 120;
    vector<Region*> inRegions;
    RectangleShape disp_rect; // for debug drawing

    bool isEnemy = false;
    float maxHealth;
    float health   ;

    GameObject() {
        activeObjects.emplace_back(this);
        localHull = rect(-50, -50, 100, 100);
    }
    // this function looks yucky, keep an eye on it
    void updateRegions();
    virtual void move() {}
    virtual void renderBack() {}
    virtual void renderFront() {}
    virtual void hurt(float damage) {}
    void renderDebug() {
        disp_rect.setFillColor(Color(0, 0, 0, 0));
        disp_rect.setOutlineColor(Color::Blue);
        disp_rect.setOutlineThickness(2);
        disp_rect.setPosition(pos.x + localHull.x, pos.y + localHull.y);
        disp_rect.setSize(Vector2f(localHull.w, localHull.h));
        window->draw(disp_rect);
    }
    virtual ~GameObject();
};

