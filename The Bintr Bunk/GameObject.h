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
    //sf::Sprite sprite; // bullet dont need no sprite
    GameObject* parent = nullptr;
    vector<GameObject*> children;
    int layer = 0;
    bool canMove = false;
    bool allowedToHearPlayer = false;

    vec pos;
    vec posLast;
    vec posInterp;
    float angle = 0;
    float angInterp;
    bool dir = true;
    rect localHull;
    int life = 120;
    vector<Region*> inRegions;
    RectangleShape disp_rect; // for debug drawing

    bool isEnemy = false;
    float maxHealth;
    float health   ;

    GameObject();
    // this function looks yucky, keep an eye on it
    void updateRegions();
    void set(vec pos, float angle, bool dir);
    // set an objects posInterp/angInterp variables
    // to current position, used later in rendering
    virtual void setInterpolation() {}
    virtual void move() {}
    //virtual void renderBack() {}
    //virtual void renderFront() {}
    virtual void render() {}
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

