#pragma once
#include "GameObject.h"
#include "BintrsTools.h"
#include "LoadResources.h"
//extern SoundBuffer* explosion;

class Grenade : public GameObject
{
public:
    vec vel;
    Grenade(vec pos, float angle, float speed) {
        this->pos = pos;
        this->vel = vec(speed, 0).rotated(angle);
        life = 60;
        updateRegions();
    }
    void move();
    void renderFront() {
        CircleShape disp_circ;
        disp_circ.setFillColor(Color::Blue);
        disp_circ.setRadius(10);
        disp_circ.setOrigin(10, 10);
        disp_circ.setPosition(pos.x, pos.y);
        window->draw(disp_circ);
    }
};
