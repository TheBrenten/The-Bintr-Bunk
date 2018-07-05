#pragma once
#include "GameObject.h"
#include "BintrsTools.h"
#include "LoadResources.h"
//extern SoundBuffer* explosion;

class Grenade : public GameObject
{
public:
    vec vel;
    Grenade(vec pos, float angle, float speed);
    vec posInterp;
    float angInterp;
    void setInterpolation();
    void move();
    void render();
};
